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
#include "pxr/usd/sdf/reference.h"
#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyUtils.h"

#include <boost/python.hpp>
#include <boost/function.hpp>

#include <string>


PXR_NAMESPACE_USING_DIRECTIVE

namespace {

static std::string
_Repr(const SdfReference &self)
{
    std::string args;
    bool useKeywordArgs = false;

    if (!self.GetAssetPath().empty()) {
        args += TfPyRepr(self.GetAssetPath());
    } else {
        useKeywordArgs = true;
    }
    if (!self.GetPrimPath().IsEmpty()) {
        args += (args.empty() ? "": ", ");
        args += (useKeywordArgs ? "primPath=" : "") +
            TfPyRepr(self.GetPrimPath());
    } else {
        useKeywordArgs = true;
    }
    if (!self.GetLayerOffset().IsIdentity()) {
        args += (args.empty() ? "": ", ");
        args += (useKeywordArgs ? "layerOffset=" : "") +
            TfPyRepr(self.GetLayerOffset());
    } else {
        useKeywordArgs = true;
    }
    // Always use keyword args for custom data (for readability).
    if (!self.GetCustomData().empty()) {
        args += (args.empty() ? "": ", ");
        args += "customData=" + TfPyRepr(self.GetCustomData());
    }

    return TF_PY_REPR_PREFIX + "Reference(" + args + ")";
}

} // anonymous namespace 

void wrapReference()
{    
    using This = SdfReference;

    // Register conversion for python list <-> vector<SdfReference>
    boost::python::to_python_converter<
        SdfReferenceVector,
        TfPySequenceToPython<SdfReferenceVector> >();
    TfPyContainerConversions::from_python_sequence<
        SdfReferenceVector,
        TfPyContainerConversions::variable_capacity_policy >();

    // Note: Since we have no proxy for Sdf.Reference we wrap it as an
    //       immutable type to avoid confusion about code like this
    //       prim.referenceList.explicitItems[0].assetPath = '//menv30/test.menva'
    //       This looks like it's updating the assetPath for the prim's
    //       first explicit reference, but would instead modify a temporary
    //       Sdf.Reference object.

    boost::python::class_<This>( "Reference" )
        .def(boost::python::init<const std::string &,
                  const SdfPath &,
                  const SdfLayerOffset &,
                  const VtDictionary &>(
            ( boost::python::arg("assetPath") = std::string(),
              boost::python::arg("primPath") = SdfPath(),
              boost::python::arg("layerOffset") = SdfLayerOffset(),
              boost::python::arg("customData") = VtDictionary(0) ) ) )
        .def(boost::python::init<const This &>())

        .add_property("assetPath",
            boost::python::make_function(
                &This::GetAssetPath, boost::python::return_value_policy<boost::python::return_by_value>()))
        .add_property("primPath",
            boost::python::make_function(
                &This::GetPrimPath, boost::python::return_value_policy<boost::python::return_by_value>()))
        .add_property("layerOffset",
            boost::python::make_function(
                &This::GetLayerOffset, boost::python::return_value_policy<boost::python::return_by_value>()))
        .add_property("customData",
            boost::python::make_function(
                &This::GetCustomData, boost::python::return_value_policy<boost::python::return_by_value>()))

        .def("IsInternal", &This::IsInternal)

        .def(boost::python::self == boost::python::self)
        .def(boost::python::self != boost::python::self)
        .def(boost::python::self < boost::python::self)
        .def(boost::python::self > boost::python::self)
        .def(boost::python::self <= boost::python::self)
        .def(boost::python::self >= boost::python::self)

        .def("__repr__", _Repr)

        ;

}
