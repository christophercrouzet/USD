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
#include "pxr/usd/usdPhysics/driveAPI.h"
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

namespace pxrUsdUsdPhysicsWrapDriveAPI {

#define WRAP_CUSTOM                                                     \
    template <class Cls> static void _CustomWrapCode(Cls &_class)

// fwd decl.
WRAP_CUSTOM;

        
static UsdAttribute
_CreateTypeAttr(UsdPhysicsDriveAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateTypeAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Token), writeSparsely);
}
        
static UsdAttribute
_CreateMaxForceAttr(UsdPhysicsDriveAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateMaxForceAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateTargetPositionAttr(UsdPhysicsDriveAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateTargetPositionAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateTargetVelocityAttr(UsdPhysicsDriveAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateTargetVelocityAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateDampingAttr(UsdPhysicsDriveAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateDampingAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateStiffnessAttr(UsdPhysicsDriveAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateStiffnessAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}

static bool _WrapIsPhysicsDriveAPIPath(const SdfPath &path) {
    TfToken collectionName;
    return UsdPhysicsDriveAPI::IsPhysicsDriveAPIPath(
        path, &collectionName);
}

static std::string
_Repr(const UsdPhysicsDriveAPI &self)
{
    std::string primRepr = TfPyRepr(self.GetPrim());
    std::string instanceName = self.GetName();
    return TfStringPrintf(
        "UsdPhysics.DriveAPI(%s, '%s')",
        primRepr.c_str(), instanceName.c_str());
}

struct UsdPhysicsDriveAPI_CanApplyResult : 
    public TfPyAnnotatedBoolResult<std::string>
{
    UsdPhysicsDriveAPI_CanApplyResult(bool val, std::string const &msg) :
        TfPyAnnotatedBoolResult<std::string>(val, msg) {}
};

static UsdPhysicsDriveAPI_CanApplyResult
_WrapCanApply(const UsdPrim& prim, const TfToken& name)
{
    std::string whyNot;
    bool result = UsdPhysicsDriveAPI::CanApply(prim, name, &whyNot);
    return UsdPhysicsDriveAPI_CanApplyResult(result, whyNot);
}

} // anonymous namespace

void wrapUsdPhysicsDriveAPI()
{
    typedef UsdPhysicsDriveAPI This;

    pxrUsdUsdPhysicsWrapDriveAPI::UsdPhysicsDriveAPI_CanApplyResult::Wrap<pxrUsdUsdPhysicsWrapDriveAPI::UsdPhysicsDriveAPI_CanApplyResult>(
        "_CanApplyResult", "whyNot");

    boost::python::class_<This, boost::python::bases<UsdAPISchemaBase> >
        cls("DriveAPI");

    cls
        .def(boost::python::init<UsdPrim, TfToken>())
        .def(boost::python::init<UsdSchemaBase const&, TfToken>())
        .def(TfTypePythonClass())

        .def("Get",
            (UsdPhysicsDriveAPI(*)(const UsdStagePtr &stage, 
                                       const SdfPath &path))
               &This::Get,
            (boost::python::arg("stage"), boost::python::arg("path")))
        .def("Get",
            (UsdPhysicsDriveAPI(*)(const UsdPrim &prim,
                                       const TfToken &name))
               &This::Get,
            (boost::python::arg("prim"), boost::python::arg("name")))
        .staticmethod("Get")

        .def("CanApply", &pxrUsdUsdPhysicsWrapDriveAPI::_WrapCanApply, (boost::python::arg("prim"), boost::python::arg("name")))
        .staticmethod("CanApply")

        .def("Apply", &This::Apply, (boost::python::arg("prim"), boost::python::arg("name")))
        .staticmethod("Apply")

        .def("GetSchemaAttributeNames",
             (const TfTokenVector &(*)(bool))&This::GetSchemaAttributeNames,
             boost::python::arg("includeInherited")=true,
             boost::python::return_value_policy<TfPySequenceToList>())
        .def("GetSchemaAttributeNames",
             (TfTokenVector(*)(bool, const TfToken &))
                &This::GetSchemaAttributeNames,
             boost::python::arg("includeInherited"),
             boost::python::arg("instanceName"),
             boost::python::return_value_policy<TfPySequenceToList>())
        .staticmethod("GetSchemaAttributeNames")

        .def("_GetStaticTfType", (TfType const &(*)()) TfType::Find<This>,
             boost::python::return_value_policy<boost::python::return_by_value>())
        .staticmethod("_GetStaticTfType")

        .def(!boost::python::self)

        
        .def("GetTypeAttr",
             &This::GetTypeAttr)
        .def("CreateTypeAttr",
             &pxrUsdUsdPhysicsWrapDriveAPI::_CreateTypeAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetMaxForceAttr",
             &This::GetMaxForceAttr)
        .def("CreateMaxForceAttr",
             &pxrUsdUsdPhysicsWrapDriveAPI::_CreateMaxForceAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetTargetPositionAttr",
             &This::GetTargetPositionAttr)
        .def("CreateTargetPositionAttr",
             &pxrUsdUsdPhysicsWrapDriveAPI::_CreateTargetPositionAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetTargetVelocityAttr",
             &This::GetTargetVelocityAttr)
        .def("CreateTargetVelocityAttr",
             &pxrUsdUsdPhysicsWrapDriveAPI::_CreateTargetVelocityAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetDampingAttr",
             &This::GetDampingAttr)
        .def("CreateDampingAttr",
             &pxrUsdUsdPhysicsWrapDriveAPI::_CreateDampingAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetStiffnessAttr",
             &This::GetStiffnessAttr)
        .def("CreateStiffnessAttr",
             &pxrUsdUsdPhysicsWrapDriveAPI::_CreateStiffnessAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))

        .def("IsPhysicsDriveAPIPath", pxrUsdUsdPhysicsWrapDriveAPI::_WrapIsPhysicsDriveAPIPath)
            .staticmethod("IsPhysicsDriveAPIPath")
        .def("__repr__", pxrUsdUsdPhysicsWrapDriveAPI::_Repr)
    ;

    pxrUsdUsdPhysicsWrapDriveAPI::_CustomWrapCode(cls);
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

namespace pxrUsdUsdPhysicsWrapDriveAPI {

WRAP_CUSTOM {
}

}
