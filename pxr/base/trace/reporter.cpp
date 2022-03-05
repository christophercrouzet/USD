//
// Copyright 2018 Pixar
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

#include "pxr/base/trace/reporter.h"

#include "pxr/pxr.h"
#include "pxr/base/trace/aggregateTree.h"
#include "pxr/base/trace/collector.h"
#include "pxr/base/trace/eventTree.h"
#include "pxr/base/trace/reporterDataSourceCollector.h"
#include "pxr/base/trace/threads.h"

#include "pxr/base/tf/mallocTag.h"
#include "pxr/base/tf/stringUtils.h"
#include "pxr/base/arch/timing.h"
#include "pxr/base/js/json.h"

#include <algorithm>
#include <map>
#include <ostream>
#include <vector>


PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_PUBLIC_TOKENS(TraceReporterTokens, TRACE_REPORTER_TOKENS);

//
// TraceReporter
//

TraceReporter::TraceReporter(const std::string& label,
                             DataSourcePtr dataSource) :
    TraceReporterBase(std::move(dataSource)),
    _label(label),
    _groupByFunction(true),
    _foldRecursiveCalls(false),
    _shouldAdjustForOverheadAndNoise(true)
{
    _aggregateTree = TraceAggregateTree::New();
    _eventTree = TraceEventTree::New();
}

TraceReporter::~TraceReporter()
{
}

static std::string
_IndentString(int indent)
{
    std::string s;
    s.resize(indent, ' ');

    // Insert '|' characters every 4 spaces.
    // The magic value of 2 makes it line up with the outer scope.
    for (int i=2; i<indent; i+=4) {
        s[i] = '|';
    }
    return s;
}

static std::string
_GetKeyName(const TfToken& key)
{
    return key.GetString();
}

static void
_PrintLineTimes(std::ostream &s, double inclusive, double exclusive,
                int count, const std::string& label, int indent,
                bool recursive_node, int iterationCount)
{
    std::string inclusiveStr = TfStringPrintf("%9.3f ms ",
            ArchTicksToSeconds( uint64_t(inclusive * 1e3) / iterationCount ));
    if (inclusive <= 0)
        inclusiveStr = std::string(inclusiveStr.size(), ' ');

    std::string exclusiveStr = TfStringPrintf("%9.3f ms ",
            ArchTicksToSeconds( uint64_t(exclusive * 1e3) / iterationCount ));
    if (exclusive <= 0)
        exclusiveStr = std::string(exclusiveStr.size(), ' ');

    std::string countStr;
    if (iterationCount == 1)
        countStr = TfStringPrintf("%7.0f samples ", double(count));
    else
        countStr = TfStringPrintf("%10.3f samples ", 
                                  double(count)/iterationCount);

    if (count <= 0)
    {
        // CODE_COVERAGE_OFF -- shouldn't get a count of zero
        countStr = std::string(countStr.size(), ' ');
        // CODE_COVERAGE_ON
    }

    s << inclusiveStr << exclusiveStr << countStr << " ";

    s << _IndentString(indent);

    // Put a '*' before the label of recursive nodes so that we can easily 
    // identify them.
    if (recursive_node) 
        s << "*";
    s << label << "\n";
}

static void
_PrintRecursionMarker(
    std::ostream &s,
    const std::string &label, 
    int indent)
{
    std::string inclusiveStr(13, ' ');
    std::string exclusiveStr(13, ' ');
    std::string countStr(16, ' ');

    // Need -1 here in order to get '|' characters to line up.
    std::string indentStr( _IndentString(indent-1) );

    s << inclusiveStr << exclusiveStr << countStr << " " << indentStr << " ";
    s << "[" << label << "]\n";

}

static void
_PrintNodeTimes(
    std::ostream &s,
    TraceAggregateNodeRefPtr node,
    int indent, 
    int iterationCount)
{
    // The root of the tree has id == -1, no useful stats there.

    if (node->GetId().IsValid()) {

        if (node->IsRecursionMarker()) {
            _PrintRecursionMarker(s, _GetKeyName(node->GetKey()), indent);
            return;
        }

        bool r = node->IsRecursionHead();
        _PrintLineTimes(s, node->GetInclusiveTime(), node->GetExclusiveTime(r),
                        node->GetCount(r), _GetKeyName(node->GetKey()),
                        indent, r, iterationCount);
    }

    // sort children by inclusive time on output
    std::vector<TraceAggregateNodeRefPtr> sortedKids;
    for (const TraceAggregateNodeRefPtr& it : node->GetChildrenRef()) {
        sortedKids.push_back(it);
    }
    
    for (const TraceAggregateNodeRefPtr& it : sortedKids) {
        _PrintNodeTimes(s, it, indent+2, iterationCount);
    }
}

void
TraceReporter::_PrintTimes(std::ostream &s)
{
    using SortedTimes = std::multimap<TimeStamp, TfToken>;

    SortedTimes sortedTimes;
    for (const TraceAggregateTree::EventTimes::value_type& it
            : _aggregateTree->GetEventTimes() ) {
        sortedTimes.insert(SortedTimes::value_type(it.second, it.first));
    }
    for (const SortedTimes::value_type& it : sortedTimes) {
        s << TfStringPrintf("%9.3f ms ",
                            ArchTicksToSeconds((uint64_t)(it.first * 1e3)))
          << _GetKeyName(it.second) << "\n";
    }
}

void
TraceReporter::Report(
    std::ostream &s,
    int iterationCount)
{
    if (iterationCount < 1) {
        TF_CODING_ERROR("iterationCount %d is invalid; falling back to 1",
                        iterationCount);
        iterationCount = 1;
    }

    UpdateTraceTrees();

    // Adjust for overhead.
    if (ShouldAdjustForOverheadAndNoise()) {
        _aggregateTree->GetRoot()->AdjustForOverheadAndNoise(
            TraceCollector::GetInstance().GetScopeOverhead(),
            ArchGetTickQuantum());
    }

    // Fold recursive calls if we need to.
    if (GetFoldRecursiveCalls()) {
        _aggregateTree->GetRoot()->MarkRecursiveChildren();
    }

    if (iterationCount > 1)
        s << "\nNumber of iterations: " << iterationCount << "\n";

    s << "\nTree view  ==============\n";
    if (iterationCount == 1)
        s << "   inclusive    exclusive        \n";
    else {
        s << "  incl./iter   excl./iter       samples/iter\n";
    }

    _PrintNodeTimes(s, _aggregateTree->GetRoot(), 0, iterationCount);

    s << "\n";
}

void
TraceReporter::ReportTimes(std::ostream &s)
{
    UpdateTraceTrees();

    s << "\nTotal time for each key ==============\n";
    _PrintTimes(s);
    s << "\n";
}

void 
TraceReporter::ReportChromeTracing(std::ostream &s)
{
    UpdateTraceTrees();

    JsWriter w(s);
    _eventTree->WriteChromeTraceObject(w);
}


void 
TraceReporter::_RebuildEventAndAggregateTrees()
{
    // Get the latest from the collector and process the events.
    _Update();

    // If MallocTags were enabled for the capture of this trace, add a dummy
    // warning node as an indicator that the trace may have been slowed down
    // by the memory tagging, unless there was nothing reported anyway.
    // XXX: add "WARNING" token that Spy can use.
    TraceAggregateNodePtr root = _aggregateTree->GetRoot();
    if (root && !root->GetChildrenRef().empty() && 
        TfMallocTag::IsInitialized()) {
        root->Append(TraceAggregateNode::Id(), 
                          TfToken(
                              TraceReporterTokens->warningString.GetString() +
                              " MallocTags enabled"),
                          0,
                          1   /* count */,
                          1   /* exclusive count */);
    }
}

void
TraceReporter::UpdateTraceTrees()
{
    _RebuildEventAndAggregateTrees();
}

void 
TraceReporter::ClearTree() 
{ 
    _aggregateTree->Clear();
    _eventTree = TraceEventTree::New();
    _Clear();
}

TraceAggregateNodePtr
TraceReporter::GetAggregateTreeRoot()
{
    return _aggregateTree->GetRoot();
}

TraceEventNodeRefPtr
TraceReporter::GetEventRoot()
{
    return _eventTree->GetRoot();
}

TraceEventTreeRefPtr
TraceReporter::GetEventTree()
{
    return _eventTree;
}


const TraceReporter::CounterMap &
TraceReporter::GetCounters() const
{
    return _aggregateTree->GetCounters();
}

int
TraceReporter::GetCounterIndex(const TfToken &key) const
{
    return _aggregateTree->GetCounterIndex(key);
}

bool
TraceReporter::AddCounter(const TfToken &key, int index, double totalValue)
{
    return _aggregateTree->AddCounter(key, index, totalValue);
}

void
TraceReporter::SetGroupByFunction(bool groupByFunction)
{
    _groupByFunction = groupByFunction;
}

bool
TraceReporter::GetGroupByFunction() const
{
    return _groupByFunction;
}

void 
TraceReporter::SetFoldRecursiveCalls(bool foldRecursiveCalls)
{
    _foldRecursiveCalls = foldRecursiveCalls;
}

bool 
TraceReporter::GetFoldRecursiveCalls() const
{
    return _foldRecursiveCalls;
}

void
TraceReporter::SetShouldAdjustForOverheadAndNoise(bool shouldAdjust)
{
    _shouldAdjustForOverheadAndNoise = shouldAdjust;
}

bool
TraceReporter::ShouldAdjustForOverheadAndNoise() const
{
    return _shouldAdjustForOverheadAndNoise;
}

/* static */
TraceAggregateNode::Id
TraceReporter::CreateValidEventId() 
{
    return TraceAggregateNode::Id(TraceGetThreadId());
}

void
TraceReporter::_ProcessCollection(
    const TraceReporterBase::CollectionPtr& collection)
{
    if (collection) {

        // We just always build the single (additional) event tree for the 
        // (additional) new collection given and pass it on to the aggregate 
        // tree. Note that the call to Add() merges in the newGraph to 
        // _eventTree which thus represents the merged sum of all collections 
        // seen here whereas newGraph is just the graph for the new collection. 

        TraceEventTreeRefPtr newGraph = _eventTree->Add(*collection);
        _aggregateTree->Append(newGraph, *collection);
    }
}

TraceReporterPtr 
TraceReporter::GetGlobalReporter()
{
    // Note that, like TfSingleton, the global reporter instance is not freed
    // at shutdown.
    static const TraceReporterPtr globalReporter(
        new TraceReporter(
            "Trace global reporter",
            TraceReporterDataSourceCollector::New()));
    return globalReporter;
}

PXR_NAMESPACE_CLOSE_SCOPE
