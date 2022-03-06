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
////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// wrapQuat.template.cpp file to make changes.

#include "pxr/pxr.h"
#include "pxr/base/gf/quatd.h"
#include "pxr/base/gf/quatf.h"
#include "pxr/base/gf/quath.h"

#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"
#include "pxr/base/tf/pyContainerConversions.h"

#include <boost/python/class.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/def.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/return_arg.hpp>

#include <string>



PXR_NAMESPACE_USING_DIRECTIVE

namespace pxrBaseGfWrapQuath {

static std::string __repr__(GfQuath const &self) {
    return TF_PY_REPR_PREFIX + "Quath(" +
        TfPyRepr(self.GetReal()) + ", " +
        TfPyRepr(self.GetImaginary()) + ")";
}

#if PY_MAJOR_VERSION == 2
static GfQuath __truediv__(const GfQuath &self, GfHalf value)
{
    return self / value;
}

static GfQuath __itruediv__(GfQuath &self, GfHalf value)
{
    return self /= value;
}
#endif

// Zero-initialized default ctor for python.
static GfQuath *__init__() { return new GfQuath(0); }

} // anonymous namespace 

void wrapQuath()
{    
    boost::python::object getImaginary =
        boost::python::make_function(&GfQuath::GetImaginary,
                      boost::python::return_value_policy<boost::python::return_by_value>());

    boost::python::object setImaginaryVec =
        boost::python::make_function((void (GfQuath::*)(const GfVec3h &))
                      &GfQuath::SetImaginary);

    boost::python::object setImaginaryScl =
        boost::python::make_function((void (GfQuath::*)(GfHalf, GfHalf, GfHalf))
                      &GfQuath::SetImaginary,
                      boost::python::default_call_policies(),
                      (boost::python::arg("i"), boost::python::arg("j"), boost::python::arg("k")));

    boost::python::def("Slerp",
        (GfQuath (*)(double, const GfQuath&, const GfQuath&))
        GfSlerp);

    boost::python::def("Dot",
        (GfHalf (*)(const GfQuath&, const GfQuath&))
        GfDot);
    
    boost::python::class_<GfQuath>("Quath", boost::python::no_init)
        .def("__init__", boost::python::make_constructor(pxrBaseGfWrapQuath::__init__))
                          
        .def(TfTypePythonClass())

        .def(boost::python::init<GfQuath>())
        .def(boost::python::init<GfHalf>(boost::python::arg("real")))
        .def(boost::python::init<GfHalf, const GfVec3h &>(
                 (boost::python::arg("real"), boost::python::arg("imaginary"))))
        .def(boost::python::init<GfHalf, GfHalf, GfHalf, GfHalf>(
                 (boost::python::arg("real"), boost::python::arg("i"), boost::python::arg("j"), boost::python::arg("k"))))
        .def(boost::python::init<const GfQuatd & >())
        .def(boost::python::init<const GfQuatf & >())

        .def("GetIdentity", &GfQuath::GetIdentity)
        .staticmethod("GetIdentity")

        .def("GetReal", &GfQuath::GetReal)
        .def("SetReal", &GfQuath::SetReal)
        .add_property("real", &GfQuath::GetReal, &GfQuath::SetReal)

        .def("GetImaginary", getImaginary)
        .def("SetImaginary", setImaginaryVec)
        .def("SetImaginary", setImaginaryScl)
        .add_property("imaginary", getImaginary, setImaginaryVec)

        .def("GetLength", &GfQuath::GetLength)

        .def("GetNormalized", &GfQuath::GetNormalized,
             (boost::python::arg("eps")=GF_MIN_VECTOR_LENGTH))
        .def("Normalize", &GfQuath::Normalize,
             (boost::python::arg("eps")=GF_MIN_VECTOR_LENGTH), boost::python::return_self<>())

        .def("GetConjugate", &GfQuath::GetConjugate)
        .def("GetInverse", &GfQuath::GetInverse)

        .def("Transform", &GfQuath::Transform)

        .def(boost::python::self_ns::str(boost::python::self))
        .def(-boost::python::self)
        .def(boost::python::self == boost::python::self)
        .def(boost::python::self != boost::python::self)
        .def(boost::python::self *= boost::python::self)
        .def(boost::python::self *= GfHalf())
        .def(boost::python::self /= GfHalf())
        .def(boost::python::self += boost::python::self)
        .def(boost::python::self -= boost::python::self)
        .def(boost::python::self + boost::python::self)
        .def(boost::python::self - boost::python::self)
        .def(boost::python::self * boost::python::self)
        .def(boost::python::self * GfHalf())
        .def(GfHalf() * boost::python::self)
        .def(boost::python::self / GfHalf())

#if PY_MAJOR_VERSION == 2
        // Needed only to support "from __future__ import division" in
        // python 2. In python 3 builds boost::python adds this for us.
        .def("__truediv__", __truediv__ )
        .def("__itruediv__", __itruediv__ )
#endif

        .def("__repr__", pxrBaseGfWrapQuath::__repr__)

        ;


    boost::python::to_python_converter<std::vector<GfQuath>,
        TfPySequenceToPython<std::vector<GfQuath> > >();
    
}
