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
#include <boost/python/def.hpp>

#include "pxr/usd/usdUtils/introspection.h"

#include "pxr/base/tf/pyResultConversions.h"
#include "pxr/base/tf/pyStaticTokens.h"


PXR_NAMESPACE_USING_DIRECTIVE

namespace pxrUsdUsdUtilsWrapIntrospection {

VtDictionary 
_WrapUsdUtilsComputeUsdStageStats_1(const std::string &rootLayerPath) {
    VtDictionary stats;
    UsdUtilsComputeUsdStageStats(rootLayerPath, &stats);
    return stats;    
}

VtDictionary 
_WrapUsdUtilsComputeUsdStageStats_2(const UsdStageWeakPtr &stage) {
    VtDictionary stats;
    UsdUtilsComputeUsdStageStats(stage, &stats);
    return stats;
}

} // anonymous namespace 

void wrapIntrospection()
{
    TF_PY_WRAP_PUBLIC_TOKENS("UsdStageStatsKeys", UsdUtilsUsdStageStatsKeys, 
                             USDUTILS_USDSTAGE_STATS);

    boost::python::def("ComputeUsdStageStats", pxrUsdUsdUtilsWrapIntrospection::_WrapUsdUtilsComputeUsdStageStats_1);
    boost::python::def("ComputeUsdStageStats", pxrUsdUsdUtilsWrapIntrospection::_WrapUsdUtilsComputeUsdStageStats_2);
}
