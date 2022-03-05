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
///
/// \file tf/wrapPathUtils.cpp
#include "pxr/pxr.h"
#include "pxr/base/tf/pathUtils.h"
#include "pxr/base/tf/diagnostic.h"
#include <boost/python/def.hpp>
#include <string>



PXR_NAMESPACE_USING_DIRECTIVE

namespace {

static std::string
_RealPath(std::string const &path, bool allowInaccessibleSuffix, bool raiseOnError)
{
    std::string error;
    std::string realPath = TfRealPath(path, allowInaccessibleSuffix, &error);
    if (raiseOnError && !error.empty()) {
        TF_RUNTIME_ERROR(error);
    }
    return realPath;
}

static std::string::size_type
_FindLongestAccessiblePrefix(std::string const &path)
{
    // For python, we convert npos into path's length, which makes the return
    // value correct to use in slices.
    std::string error;
    std::string::size_type result = TfFindLongestAccessiblePrefix(path, &error);

    if (!error.empty()) {
        PyErr_SetString(PyExc_OSError, error.c_str());
        boost::python::throw_error_already_set();
    }

    return result;
}

} // anonymous namespace 

void wrapPathUtils()
{
    boost::python::def("RealPath", _RealPath,
        ( boost::python::arg("path"),
          boost::python::arg("allowInaccessibleSuffix") = false,
          boost::python::arg("raiseOnError") = false ));

    boost::python::def("FindLongestAccessiblePrefix", _FindLongestAccessiblePrefix);
}
