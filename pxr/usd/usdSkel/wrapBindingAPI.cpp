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
#include "pxr/usd/usdSkel/bindingAPI.h"
#include "pxr/usd/usd/schemaBase.h"

#include "pxr/usd/sdf/primSpec.h"

#include "pxr/usd/usd/pyConversions.h"
#include "pxr/base/tf/pyAnnotatedBoolResult.h"
#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyResultConversions.h"
#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

#include <boost/python.hpp>

#include <string>


PXR_NAMESPACE_USING_DIRECTIVE

namespace pxrUsdUsdSkelWrapBindingAPI {

#define WRAP_CUSTOM                                                     \
    template <class Cls> static void _CustomWrapCode(Cls &_class)

// fwd decl.
WRAP_CUSTOM;

        
static UsdAttribute
_CreateGeomBindTransformAttr(UsdSkelBindingAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateGeomBindTransformAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Matrix4d), writeSparsely);
}
        
static UsdAttribute
_CreateJointsAttr(UsdSkelBindingAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateJointsAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->TokenArray), writeSparsely);
}
        
static UsdAttribute
_CreateJointIndicesAttr(UsdSkelBindingAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateJointIndicesAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->IntArray), writeSparsely);
}
        
static UsdAttribute
_CreateJointWeightsAttr(UsdSkelBindingAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateJointWeightsAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->FloatArray), writeSparsely);
}
        
static UsdAttribute
_CreateBlendShapesAttr(UsdSkelBindingAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateBlendShapesAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->TokenArray), writeSparsely);
}

static std::string
_Repr(const UsdSkelBindingAPI &self)
{
    std::string primRepr = TfPyRepr(self.GetPrim());
    return TfStringPrintf(
        "UsdSkel.BindingAPI(%s)",
        primRepr.c_str());
}

struct UsdSkelBindingAPI_CanApplyResult : 
    public TfPyAnnotatedBoolResult<std::string>
{
    UsdSkelBindingAPI_CanApplyResult(bool val, std::string const &msg) :
        TfPyAnnotatedBoolResult<std::string>(val, msg) {}
};

static UsdSkelBindingAPI_CanApplyResult
_WrapCanApply(const UsdPrim& prim)
{
    std::string whyNot;
    bool result = UsdSkelBindingAPI::CanApply(prim, &whyNot);
    return UsdSkelBindingAPI_CanApplyResult(result, whyNot);
}

} // anonymous namespace

void wrapUsdSkelBindingAPI()
{
    typedef UsdSkelBindingAPI This;

    pxrUsdUsdSkelWrapBindingAPI::UsdSkelBindingAPI_CanApplyResult::Wrap<pxrUsdUsdSkelWrapBindingAPI::UsdSkelBindingAPI_CanApplyResult>(
        "_CanApplyResult", "whyNot");

    boost::python::class_<This, boost::python::bases<UsdAPISchemaBase> >
        cls("BindingAPI");

    cls
        .def(boost::python::init<UsdPrim>(boost::python::arg("prim")))
        .def(boost::python::init<UsdSchemaBase const&>(boost::python::arg("schemaObj")))
        .def(TfTypePythonClass())

        .def("Get", &This::Get, (boost::python::arg("stage"), boost::python::arg("path")))
        .staticmethod("Get")

        .def("CanApply", &pxrUsdUsdSkelWrapBindingAPI::_WrapCanApply, (boost::python::arg("prim")))
        .staticmethod("CanApply")

        .def("Apply", &This::Apply, (boost::python::arg("prim")))
        .staticmethod("Apply")

        .def("GetSchemaAttributeNames",
             &This::GetSchemaAttributeNames,
             boost::python::arg("includeInherited")=true,
             boost::python::return_value_policy<TfPySequenceToList>())
        .staticmethod("GetSchemaAttributeNames")

        .def("_GetStaticTfType", (TfType const &(*)()) TfType::Find<This>,
             boost::python::return_value_policy<boost::python::return_by_value>())
        .staticmethod("_GetStaticTfType")

        .def(!boost::python::self)

        
        .def("GetGeomBindTransformAttr",
             &This::GetGeomBindTransformAttr)
        .def("CreateGeomBindTransformAttr",
             &pxrUsdUsdSkelWrapBindingAPI::_CreateGeomBindTransformAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetJointsAttr",
             &This::GetJointsAttr)
        .def("CreateJointsAttr",
             &pxrUsdUsdSkelWrapBindingAPI::_CreateJointsAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetJointIndicesAttr",
             &This::GetJointIndicesAttr)
        .def("CreateJointIndicesAttr",
             &pxrUsdUsdSkelWrapBindingAPI::_CreateJointIndicesAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetJointWeightsAttr",
             &This::GetJointWeightsAttr)
        .def("CreateJointWeightsAttr",
             &pxrUsdUsdSkelWrapBindingAPI::_CreateJointWeightsAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetBlendShapesAttr",
             &This::GetBlendShapesAttr)
        .def("CreateBlendShapesAttr",
             &pxrUsdUsdSkelWrapBindingAPI::_CreateBlendShapesAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))

        
        .def("GetAnimationSourceRel",
             &This::GetAnimationSourceRel)
        .def("CreateAnimationSourceRel",
             &This::CreateAnimationSourceRel)
        
        .def("GetSkeletonRel",
             &This::GetSkeletonRel)
        .def("CreateSkeletonRel",
             &This::CreateSkeletonRel)
        
        .def("GetBlendShapeTargetsRel",
             &This::GetBlendShapeTargetsRel)
        .def("CreateBlendShapeTargetsRel",
             &This::CreateBlendShapeTargetsRel)
        .def("__repr__", pxrUsdUsdSkelWrapBindingAPI::_Repr)
    ;

    pxrUsdUsdSkelWrapBindingAPI::_CustomWrapCode(cls);
}

// ===================================================================== //
// Feel free to add custom code below this line, it will be preserved by 
// the code generator.  The entry point for your custom code should look
// minimally like the following:
//
// WRAP_CUSTOM {
//     _class
//         .def("MyCustomMethod", ...)
//     ;
// }
//
// Of course any other ancillary or support code may be provided.
// 
// Just remember to wrap code in the appropriate delimiters:
// 'namespace {', '}'.
//
// ===================================================================== //
// --(BEGIN CUSTOM CODE)--

namespace pxrUsdUsdSkelWrapBindingAPI {


boost::python::object
_GetSkeleton(const UsdSkelBindingAPI& binding)
{
    UsdSkelSkeleton skel;
    return binding.GetSkeleton(&skel) ? boost::python::object(skel) : boost::python::object();
}


boost::python::object
_GetAnimationSource(const UsdSkelBindingAPI& binding)
{
    UsdPrim prim;
    return binding.GetAnimationSource(&prim) ? boost::python::object(prim) : boost::python::object();
}


boost::python::tuple
_ValidateJointIndices(TfSpan<const int> jointIndices,
                      size_t numJoints)
{
    std::string reason;
    bool valid = UsdSkelBindingAPI::ValidateJointIndices(
        jointIndices, numJoints, &reason);
    return boost::python::make_tuple(valid, reason);
}


WRAP_CUSTOM {
    using This = UsdSkelBindingAPI;

    _class
        .def("GetJointIndicesPrimvar", &This::GetJointIndicesPrimvar)

        .def("CreateJointIndicesPrimvar", &This::CreateJointIndicesPrimvar,
             (boost::python::arg("constant"), boost::python::arg("elementSize")=-1))

        .def("GetJointWeightsPrimvar", &This::GetJointWeightsPrimvar)

        .def("CreateJointWeightsPrimvar", &This::CreateJointWeightsPrimvar,
             (boost::python::arg("constant"), boost::python::arg("elementSize")=-1))

        .def("SetRigidJointInfluence", &This::SetRigidJointInfluence,
             (boost::python::arg("jointIndex"), boost::python::arg("weight")=1.0f))

        .def("GetSkeleton", &_GetSkeleton)

        .def("GetAnimationSource", &_GetAnimationSource)
        
        .def("GetInheritedSkeleton", &This::GetInheritedSkeleton)
        
        .def("GetInheritedAnimationSource", &This::GetInheritedAnimationSource)

        .def("ValidateJointIndices", &_ValidateJointIndices,
             (boost::python::arg("jointIndices"), boost::python::arg("numJoints")))
        .staticmethod("ValidateJointIndices")
        ;
}

} // namespace
