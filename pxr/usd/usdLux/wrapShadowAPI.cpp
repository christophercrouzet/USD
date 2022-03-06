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
#include "pxr/usd/usdLux/shadowAPI.h"
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

namespace pxrUsdUsdLuxWrapShadowAPI {

#define WRAP_CUSTOM                                                     \
    template <class Cls> static void _CustomWrapCode(Cls &_class)

// fwd decl.
WRAP_CUSTOM;

        
static UsdAttribute
_CreateShadowEnableAttr(UsdLuxShadowAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateShadowEnableAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}
        
static UsdAttribute
_CreateShadowColorAttr(UsdLuxShadowAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateShadowColorAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Color3f), writeSparsely);
}
        
static UsdAttribute
_CreateShadowDistanceAttr(UsdLuxShadowAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateShadowDistanceAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateShadowFalloffAttr(UsdLuxShadowAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateShadowFalloffAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}
        
static UsdAttribute
_CreateShadowFalloffGammaAttr(UsdLuxShadowAPI &self,
                                      boost::python::object defaultVal, bool writeSparsely) {
    return self.CreateShadowFalloffGammaAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Float), writeSparsely);
}

static std::string
_Repr(const UsdLuxShadowAPI &self)
{
    std::string primRepr = TfPyRepr(self.GetPrim());
    return TfStringPrintf(
        "UsdLux.ShadowAPI(%s)",
        primRepr.c_str());
}

struct UsdLuxShadowAPI_CanApplyResult : 
    public TfPyAnnotatedBoolResult<std::string>
{
    UsdLuxShadowAPI_CanApplyResult(bool val, std::string const &msg) :
        TfPyAnnotatedBoolResult<std::string>(val, msg) {}
};

static UsdLuxShadowAPI_CanApplyResult
_WrapCanApply(const UsdPrim& prim)
{
    std::string whyNot;
    bool result = UsdLuxShadowAPI::CanApply(prim, &whyNot);
    return UsdLuxShadowAPI_CanApplyResult(result, whyNot);
}

} // anonymous namespace

void wrapUsdLuxShadowAPI()
{
    typedef UsdLuxShadowAPI This;

    pxrUsdUsdLuxWrapShadowAPI::UsdLuxShadowAPI_CanApplyResult::Wrap<pxrUsdUsdLuxWrapShadowAPI::UsdLuxShadowAPI_CanApplyResult>(
        "_CanApplyResult", "whyNot");

    boost::python::class_<This, boost::python::bases<UsdAPISchemaBase> >
        cls("ShadowAPI");

    cls
        .def(boost::python::init<UsdPrim>(boost::python::arg("prim")))
        .def(boost::python::init<UsdSchemaBase const&>(boost::python::arg("schemaObj")))
        .def(TfTypePythonClass())

        .def("Get", &This::Get, (boost::python::arg("stage"), boost::python::arg("path")))
        .staticmethod("Get")

        .def("CanApply", &pxrUsdUsdLuxWrapShadowAPI::_WrapCanApply, (boost::python::arg("prim")))
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

        
        .def("GetShadowEnableAttr",
             &This::GetShadowEnableAttr)
        .def("CreateShadowEnableAttr",
             &pxrUsdUsdLuxWrapShadowAPI::_CreateShadowEnableAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetShadowColorAttr",
             &This::GetShadowColorAttr)
        .def("CreateShadowColorAttr",
             &pxrUsdUsdLuxWrapShadowAPI::_CreateShadowColorAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetShadowDistanceAttr",
             &This::GetShadowDistanceAttr)
        .def("CreateShadowDistanceAttr",
             &pxrUsdUsdLuxWrapShadowAPI::_CreateShadowDistanceAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetShadowFalloffAttr",
             &This::GetShadowFalloffAttr)
        .def("CreateShadowFalloffAttr",
             &pxrUsdUsdLuxWrapShadowAPI::_CreateShadowFalloffAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))
        
        .def("GetShadowFalloffGammaAttr",
             &This::GetShadowFalloffGammaAttr)
        .def("CreateShadowFalloffGammaAttr",
             &pxrUsdUsdLuxWrapShadowAPI::_CreateShadowFalloffGammaAttr,
             (boost::python::arg("defaultValue")=boost::python::object(),
              boost::python::arg("writeSparsely")=false))

        .def("__repr__", pxrUsdUsdLuxWrapShadowAPI::_Repr)
    ;

    pxrUsdUsdLuxWrapShadowAPI::_CustomWrapCode(cls);
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

#include "pxr/usd/usdShade/connectableAPI.h"

namespace pxrUsdUsdLuxWrapShadowAPI {

WRAP_CUSTOM {
    _class
        .def(boost::python::init<UsdShadeConnectableAPI>(boost::python::arg("connectable")))
        .def("ConnectableAPI", &UsdLuxShadowAPI::ConnectableAPI)

        .def("CreateOutput", &UsdLuxShadowAPI::CreateOutput,
             (boost::python::arg("name"), boost::python::arg("type")))
        .def("GetOutput", &UsdLuxShadowAPI::GetOutput, boost::python::arg("name"))
        .def("GetOutputs", &UsdLuxShadowAPI::GetOutputs,
             (boost::python::arg("onlyAuthored")=true),
             boost::python::return_value_policy<TfPySequenceToList>())

        .def("CreateInput", &UsdLuxShadowAPI::CreateInput,
             (boost::python::arg("name"), boost::python::arg("type")))
        .def("GetInput", &UsdLuxShadowAPI::GetInput, boost::python::arg("name"))
        .def("GetInputs", &UsdLuxShadowAPI::GetInputs,
             (boost::python::arg("onlyAuthored")=true),
             boost::python::return_value_policy<TfPySequenceToList>())
        ;
}

}
