//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#include "pxr/pxr.h"
#include "pxr/base/arch/attributes.h"
#include "pxr/base/arch/stackTrace.h"
#include "pxr/base/arch/vsnprintf.h"
#include "pxr/base/tf/ostreamMethods.h"
#include "pxr/base/tf/patternMatcher.h"
#include "pxr/base/tf/stringUtils.h"
#include "pxr/base/tf/scopeDescription.h"
#include "pxr/usd/sdf/layer.h"

#include <boost/program_options.hpp>

#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <vector>

PXR_NAMESPACE_OPEN_SCOPE



namespace {

std::string progName;

void VErr(char const *fmt, va_list ap) {
    fprintf(stderr, "%s: Error - %s\n", progName.c_str(),
            ArchVStringPrintf(fmt, ap).c_str());
}

void VPrint(char const *fmt, va_list ap) { vprintf(fmt, ap); }

void Err(char const *fmt, ...) ARCH_PRINTF_FUNCTION(1, 2);
void Err(char const *fmt, ...) {
    va_list ap; va_start(ap, fmt); VErr(fmt, ap); va_end(ap);
}

void ErrExit(char const *fmt, ...) ARCH_PRINTF_FUNCTION(1, 2);
void ErrExit(char const *fmt, ...) {
    va_list ap; va_start(ap, fmt); VErr(fmt, ap); va_end(ap);
    exit(1);
}

void Print(char const *fmt, ...) ARCH_PRINTF_FUNCTION(1, 2);
void Print(char const *fmt, ...) {
    va_list ap; va_start(ap, fmt); VPrint(fmt, ap); va_end(ap);
}

bool IsClose(double a, double b, double tol)
{
    double absDiff = fabs(a-b);
    return absDiff <= fabs(tol*a) || absDiff < fabs(tol*b);
}

struct SortKey {
    SortKey() : key("path") {}
    SortKey(std::string key) : key(key) {}
    std::string key;
};
std::ostream &operator<<(std::ostream &os, SortKey const &sk) { return os << sk.key; }

// boost::program_options calls validate() for custom argument types.
void validate(boost::any& v, const std::vector<std::string> &values, SortKey*, int) {
    boost::program_options::validators::check_first_occurrence(v);
    std::string const &s = boost::program_options::validators::get_single_string(values);
    if (s == "path" || s == "field") {
        v = SortKey(s);
    } else {
        throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value);
    }
}

// Parse times and time ranges in timeSpecs, throw an exception if something
// goes wrong.
void
ParseTimes(std::vector<std::string> const &timeSpecs,
           std::vector<double> *literalTimes,
           std::vector<std::pair<double, double>> *timeRanges)
{
    for (auto const &spec: timeSpecs) {
        try {
            if (TfStringContains(spec, "..")) {
                auto elts = TfStringSplit(spec, "..");
                if (elts.size() != 2) {
                    throw std::invalid_argument(
                        TfStringPrintf("invalid time syntax '%s'",
                                       spec.c_str()));
                }
                timeRanges->emplace_back(boost::lexical_cast<double>(elts[0]),
                                         boost::lexical_cast<double>(elts[1]));
            } else {
                literalTimes->emplace_back(boost::lexical_cast<double>(spec));
            }
        } catch (boost::bad_lexical_cast const &) {
            throw std::invalid_argument(
                TfStringPrintf("invalid time syntax '%s'", spec.c_str()));
        }
    }
    std::sort(literalTimes->begin(), literalTimes->end());
    literalTimes->erase(std::unique(literalTimes->begin(), literalTimes->end()),
                        literalTimes->end());
    std::sort(timeRanges->begin(), timeRanges->end());
    timeRanges->erase(std::unique(timeRanges->begin(), timeRanges->end()),
                      timeRanges->end());
}

struct ReportParams
{
    TfPatternMatcher *pathMatcher = nullptr;
    TfPatternMatcher *fieldMatcher = nullptr;
    SortKey sortKey;
    std::vector<double> literalTimes;
    std::vector<std::pair<double, double>> timeRanges;
    double timeTolerance;
    bool showSummary = false;
    bool validate = false;
    bool showValues = true;
    bool fullArrays = false;
};

struct SummaryStats
{
    size_t numSpecs = 0;
    size_t numPrimSpecs = 0;
    size_t numPropertySpecs = 0;
    size_t numFields = 0;
    size_t numSampleTimes = 0;
};

SummaryStats
GetSummaryStats(SdfLayerHandle const &layer)
{
    SummaryStats stats;
    layer->Traverse(
        SdfPath::AbsoluteRootPath(), [&stats, &layer](SdfPath const &path) {
            ++stats.numSpecs;
            stats.numPrimSpecs += path.IsPrimPath();
            stats.numPropertySpecs += path.IsPropertyPath();
            stats.numFields += layer->ListFields(path).size();
        });
    stats.numSampleTimes = layer->ListAllTimeSamples().size();
    return stats;
}

std::vector<SdfPath>
CollectPaths(SdfLayerHandle const &layer, ReportParams const &p)
{
    std::vector<SdfPath> result;
    layer->Traverse(SdfPath::AbsoluteRootPath(),
                    [&result, &p](SdfPath const &path) {
                        if (p.pathMatcher->Match(path.GetString()))
                            result.push_back(path);
                    });
    return result;
}

std::vector<TfToken>
CollectFields(SdfLayerHandle const &layer, SdfPath const &path,
              ReportParams const &p)
{
    std::vector<TfToken> fields = layer->ListFields(path);
    fields.erase(std::remove_if(fields.begin(), fields.end(),
                           [&p](TfToken const &f) {
                               return !p.fieldMatcher->Match(f.GetString());
                           }),
                 fields.end());
    return fields;
}

std::string
GetValueString(VtValue const &value, ReportParams const &p)
{
    return (!p.fullArrays && value.IsArrayValued()) ?
        TfStringPrintf("%s size %zu", value.GetTypeName().c_str(),
                       value.GetArraySize()) :
        TfStringPrintf("%s = %s", value.GetTypeName().c_str(),
                       TfStringify(value).c_str());
}

std::string
GetTimeSamplesValueString(SdfLayerHandle const &layer,
                          SdfPath const &path, ReportParams const &p)
{
    // Pull all the sample times for the given path, then collect up by p's
    // literalTimes and timeRanges, then if we're pulling values we'll do that
    // for each sample, and format.
    bool takeAllTimes = p.literalTimes.empty() && p.timeRanges.empty();
    auto times = layer->ListTimeSamplesForPath(path);
    std::vector<double> selectedTimes;
    selectedTimes.reserve(times.size());

    if (takeAllTimes) {
        selectedTimes.assign(times.begin(), times.end());
    }
    else {
        for (auto time: times) {
            // Check literalTimes.
            auto rng = std::equal_range(
                p.literalTimes.begin(), p.literalTimes.end(), time,
                [&p](double a, double b)  {
                    return IsClose(a, b, p.timeTolerance) ? false : a < b;
                });
            if (rng.first != rng.second)
                selectedTimes.push_back(time);

            // Check ranges.
            for (auto const &range: p.timeRanges) {
                if (range.first <= time && time <= range.second)
                    selectedTimes.push_back(time);
            }
        }
    }

    if (selectedTimes.empty())
        return "<no samples at requested times>";

    std::vector<std::string> result;
    VtValue val;
    for (auto time: selectedTimes) {
        TF_VERIFY(layer->QueryTimeSample(path, time, &val));
        result.push_back(
            TfStringPrintf("%s: %s", TfStringify(time).c_str(),
                           GetValueString(val, p).c_str()));
    }
    return TfStringPrintf("[\n        %s ]",
                          TfStringJoin(result, "\n        ").c_str());
}
                          
std::string
GetFieldValueString(SdfLayerHandle const &layer,
                    SdfPath const &path, TfToken const &field,
                    ReportParams const &p)
{
    std::string result;
    // Handle timeSamples specially:
    if (field == SdfFieldKeys->TimeSamples) {
        result = GetTimeSamplesValueString(layer, path, p);
    } else {
        VtValue value;
        TF_VERIFY(layer->HasField(path, field, &value));
        result = GetValueString(value, p);
    }
    return result;
}

void
GetReportByPath(SdfLayerHandle const &layer, ReportParams const &p,
                std::vector<std::string> &report)
{
    std::vector<SdfPath> paths = CollectPaths(layer, p);
    std::sort(paths.begin(), paths.end());
    for (auto const &path: paths) {
        SdfSpecType specType = layer->GetSpecType(path);
        report.push_back(
            TfStringPrintf(
                "<%s> : %s", 
                path.GetText(), TfStringify(specType).c_str()));

        std::vector<TfToken> fields = CollectFields(layer, path, p);
        if (fields.empty())
            continue;
        for (auto const &field: fields) {
            if (p.showValues) {
                report.push_back(
                    TfStringPrintf(
                        "  %s: %s", field.GetText(),
                        GetFieldValueString(layer, path, field, p).c_str())
                    );
            } else {
                report.push_back(TfStringPrintf("  %s", field.GetText()));
            }
        }
    }
}

void
GetReportByField(SdfLayerHandle const &layer, ReportParams const &p,
                 std::vector<std::string> &report)
{
    std::vector<SdfPath> paths = CollectPaths(layer, p);
    std::unordered_map<std::string, std::vector<std::string>> pathsByFieldString;
    std::unordered_set<std::string> allFieldStrings;
    std::sort(paths.begin(), paths.end());
    for (auto const &path: paths) {
        std::vector<TfToken> fields = CollectFields(layer, path, p);
        if (fields.empty())
            continue;
        for (auto const &field: fields) {
            std::string fieldString;
            if (p.showValues) {
                fieldString = TfStringPrintf(
                    "%s: %s", field.GetText(),
                    GetFieldValueString(layer, path, field, p).c_str());
            } else {
                fieldString = TfStringPrintf("%s", field.GetText());
            }
            pathsByFieldString[fieldString].push_back(
                TfStringPrintf("  <%s>", path.GetText()));
            allFieldStrings.insert(fieldString);
        }
    }
    std::vector<std::string> fsvec(allFieldStrings.begin(), allFieldStrings.end());
    std::sort(fsvec.begin(), fsvec.end());

    for (auto const &fs: fsvec) {
        report.push_back(fs);
        auto const &ps = pathsByFieldString[fs];
        report.insert(report.end(), ps.begin(), ps.end());
    }
}


void
Validate(SdfLayerHandle const &layer, ReportParams const &p,
         std::vector<std::string> &report)
{
    TfErrorMark m;
    TF_DESCRIBE_SCOPE("Collecting paths in @%s@",
                      layer->GetIdentifier().c_str());
    std::vector<SdfPath> paths;
    layer->Traverse(SdfPath::AbsoluteRootPath(),
                    [&paths, layer](SdfPath const &path) {
                        TF_DESCRIBE_SCOPE(
                            "Collecting path <%s> in @%s@",
                            path.GetText(), layer->GetIdentifier().c_str());
                        paths.push_back(path);
                    });
    std::sort(paths.begin(), paths.end());
    for (auto const &path: paths) {
        TF_DESCRIBE_SCOPE("Collecting fields for <%s> in @%s@",
                     path.GetText(), layer->GetIdentifier().c_str());
        std::vector<TfToken> fields = layer->ListFields(path);
        if (fields.empty())
            continue;
        for (auto const &field: fields) {
            VtValue value;
            if (field == SdfFieldKeys->TimeSamples) {
                // Pull each sample value individually.
                TF_DESCRIBE_SCOPE(
                    "Getting sample times for '%s' on <%s> in @%s@",
                    field.GetText(), path.GetText(),
                    layer->GetIdentifier().c_str());
                auto times = layer->ListTimeSamplesForPath(path);

                for (auto time: times) {
                    TF_DESCRIBE_SCOPE("Getting sample value at time "
                                      "%f for '%s' on <%s> in @%s@",
                                      time, field.GetText(), path.GetText(),
                                      layer->GetIdentifier().c_str());
                    layer->QueryTimeSample(path, time, &value);
                }
            } else {
                // Just pull value.
                TF_DESCRIBE_SCOPE("Getting value for '%s' on <%s> in @%s@",
                                  field.GetText(), path.GetText(),
                                  layer->GetIdentifier().c_str());
                layer->HasField(path, field, &value);
            }
        }
    }
    report.back() += m.IsClean() ? " - OK" : " - ERROR";
}

void Report(SdfLayerHandle layer, ReportParams const &p)
{
    std::vector<std::string> report = {
        TfStringPrintf("@%s@", layer->GetIdentifier().c_str()) };
    if (p.showSummary) {
        auto stats = GetSummaryStats(layer);
        report.push_back(
            TfStringPrintf(
                "  %zu specs, %zu prim specs, %zu property specs, %zu fields, "
                "%zu sample times",
                stats.numSpecs, stats.numPrimSpecs, stats.numPropertySpecs,
                stats.numFields, stats.numSampleTimes));
    } else if (p.validate) {
        Validate(layer, p, report);
    } else if (p.sortKey.key == "path") {
        GetReportByPath(layer, p, report);
    } else if (p.sortKey.key == "field") {
        GetReportByField(layer, p, report);
    }

    for (auto const &str: report) {
        Print("%s\n", str.c_str());
    }
}

} // anon

PXR_NAMESPACE_CLOSE_SCOPE

int
main(int argc, char const *argv[])
{
    PXR_NAMESPACE_USING_DIRECTIVE

    progName = TfGetBaseName(argv[0]);

    bool showSummary = false, validate = false,
        fullArrays = false, noValues = false;
    SortKey sortKey("path");
    std::string pathRegex = ".*", fieldRegex = ".*";
    std::vector<std::string> timeSpecs, inputFiles;
    std::vector<double> literalTimes;
    std::vector<std::pair<double, double>> timeRanges;
    double timeTolerance = 1.25e-4; // ugh -- chosen to print well in help.

    boost::program_options::options_description argOpts("Options");
    argOpts.add_options()
        ("help,h", "Show help message.")
        ("summary,s", boost::program_options::bool_switch(&showSummary),
         "Report a high-level summary.")
        ("validate", boost::program_options::bool_switch(&validate),
         "Check validity by trying to read all data values.")
        ("path,p", boost::program_options::value<std::string>(&pathRegex)->value_name("regex"),
         "Report only paths matching this regex.")
        ("field,f", boost::program_options::value<std::string>(&fieldRegex)->value_name("regex"),
         "Report only fields matching this regex.")
        ("time,t", boost::program_options::value<std::vector<std::string>>(&timeSpecs)->
         multitoken()->value_name("n or ff..lf"),
         "Report only these times or time ranges for 'timeSamples' fields.")
        ("timeTolerance", boost::program_options::value<double>(&timeTolerance)->
         default_value(timeTolerance)->value_name("tol"),
         "Report times that are close to those requested within this "
         "relative tolerance.")
        ("sortBy", boost::program_options::value<SortKey>(&sortKey)->default_value(sortKey)->
         value_name("path|field"),
         "Group output by either path or field.")
        ("noValues", boost::program_options::bool_switch(&noValues),
         "Do not report field values.")
        ("fullArrays", boost::program_options::bool_switch(&fullArrays),
         "Report full array contents rather than number of elements.")
        ;

    boost::program_options::options_description inputFile("Input");
    inputFile.add_options()
        ("input-file", boost::program_options::value<std::vector<std::string>>(&inputFiles), "input files");

    boost::program_options::options_description allOpts;
    allOpts.add(argOpts).add(inputFile);

    boost::program_options::variables_map vm;
    try {
        boost::program_options::positional_options_description p;
        p.add("input-file", -1);
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).
                  options(allOpts).positional(p).run(), vm);
        boost::program_options::notify(vm);
        ParseTimes(timeSpecs, &literalTimes, &timeRanges);
    } catch (std::exception const &e) {
        ErrExit("%s", e.what());
    }

    if (vm.count("help") || inputFiles.empty()) {
        fprintf(stderr, "Usage: %s [options] <input file>\n", progName.c_str());
        fprintf(stderr, "%s\n", TfStringify(argOpts).c_str());
        exit(1);
    }

    TfPatternMatcher pathMatcher(pathRegex);
    if (!pathMatcher.IsValid()) {
        ErrExit("path regex '%s' : %s", pathRegex.c_str(),
                pathMatcher.GetInvalidReason().c_str());
    }

    TfPatternMatcher fieldMatcher(fieldRegex);
    if (!fieldMatcher.IsValid()) {
        ErrExit("field regex '%s' : %s", fieldRegex.c_str(),
                fieldMatcher.GetInvalidReason().c_str());
    }
                
    ReportParams params;
    params.showSummary = showSummary;
    params.validate = validate;
    params.pathMatcher = &pathMatcher;
    params.fieldMatcher = &fieldMatcher;
    params.sortKey = sortKey;
    params.literalTimes = literalTimes;
    params.timeRanges = timeRanges;
    params.showValues = !noValues;
    params.fullArrays = fullArrays;
    params.timeTolerance = timeTolerance;

    for (auto const &file: inputFiles) {
        TF_DESCRIBE_SCOPE("Opening layer @%s@", file.c_str());
        auto layer = SdfLayer::FindOrOpen(file);
        if (!layer) {
            Err("failed to open layer <%s>", file.c_str());
            continue;
        }
        Report(layer, params);
    }
    
    return 0;
}
