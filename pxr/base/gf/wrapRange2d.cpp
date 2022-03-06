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
// wrapRange.template.cpp file to make changes.

#include "pxr/pxr.h"
#include "pxr/base/gf/range2d.h"
#include "pxr/base/gf/range2f.h"

#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"
#include "pxr/base/tf/pyContainerConversions.h"

#include <boost/python/class.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/return_arg.hpp>

#include <string>



PXR_NAMESPACE_USING_DIRECTIVE

namespace pxrBaseGfWrapRange2d {

static const int _dimension = 2;

static std::string _Repr(GfRange2d const &self) {
    return TF_PY_REPR_PREFIX + "Range2d(" +
        TfPyRepr(self.GetMin()) + ", " + TfPyRepr(self.GetMax()) + ")";
}

#if PY_MAJOR_VERSION == 2
static GfRange2d __truediv__(const GfRange2d &self, double value)
{
    return self / value;
}

static GfRange2d __itruediv__(GfRange2d &self, double value)
{
    return self /= value;
}
#endif

static size_t __hash__(GfRange2d const &r) { return hash_value(r); }

} // anonymous namespace 

void wrapRange2d()
{    
    boost::python::object getMin = boost::python::make_function(&GfRange2d::GetMin,
                                  boost::python::return_value_policy<boost::python::return_by_value>());

    boost::python::object getMax = boost::python::make_function(&GfRange2d::GetMax,
                                  boost::python::return_value_policy<boost::python::return_by_value>());

    boost::python::class_<GfRange2d>("Range2d", boost::python::init<>())
        .def(boost::python::init<GfRange2d>())
        .def(boost::python::init<const GfVec2d &, const GfVec2d &>())
        
        .def(TfTypePythonClass())

        .def_readonly("dimension", pxrBaseGfWrapRange2d::_dimension)
        
        .add_property("min", getMin, &GfRange2d::SetMin)
        .add_property("max", getMax, &GfRange2d::SetMax)

        .def("GetMin", getMin)
        .def("GetMax", getMax)

        .def("GetSize", &GfRange2d::GetSize)
        .def("GetMidpoint", &GfRange2d::GetMidpoint)
    
        .def("SetMin", &GfRange2d::SetMin)
        .def("SetMax", &GfRange2d::SetMax)
    
        .def("IsEmpty", &GfRange2d::IsEmpty)
    
        .def("SetEmpty", &GfRange2d::SetEmpty)

        .def("Contains", (bool (GfRange2d::*)(const GfVec2d &) const)
             &GfRange2d::Contains)
        .def("Contains", (bool (GfRange2d::*)(const GfRange2d &) const)
             &GfRange2d::Contains)
    
        .def("GetUnion", &GfRange2d::GetUnion)
        .staticmethod("GetUnion")
    
        .def("UnionWith", (const GfRange2d & (GfRange2d::*)(const GfVec2d &))
             &GfRange2d::UnionWith, boost::python::return_self<>())
        .def("UnionWith", (const GfRange2d & (GfRange2d::*)(const GfRange2d &))
             &GfRange2d::UnionWith, boost::python::return_self<>())
    
        .def("GetIntersection", &GfRange2d::GetIntersection)
        .staticmethod("GetIntersection")
    
        .def("IntersectWith", (const GfRange2d & (GfRange2d::*)(const GfRange2d &))
             &GfRange2d::IntersectWith, boost::python::return_self<>())
    
        .def("GetDistanceSquared", &GfRange2d::GetDistanceSquared)
    
        .def(boost::python::self_ns::str(boost::python::self))
        .def(boost::python::self += boost::python::self)
        .def(boost::python::self -= boost::python::self)
        .def(boost::python::self *= double())
        .def(boost::python::self /= double())
        .def(boost::python::self + boost::python::self)
        .def(boost::python::self - boost::python::self)
        .def(double() * boost::python::self)
        .def(boost::python::self * double())
        .def(boost::python::self / double())
        .def(boost::python::self == GfRange2f())
        .def(boost::python::self != GfRange2f())
        .def(boost::python::self == boost::python::self)
        .def(boost::python::self != boost::python::self)
    
#if PY_MAJOR_VERSION == 2
        // Needed only to support "from __future__ import division" in
        // python 2. In python 3 builds boost::python adds this for us.
        .def("__truediv__", pxrBaseGfWrapRange2d::__truediv__ )
        .def("__itruediv__", pxrBaseGfWrapRange2d::__itruediv__ )
#endif

        .def("__repr__", pxrBaseGfWrapRange2d::_Repr)
        .def("__hash__", pxrBaseGfWrapRange2d::__hash__)

        .def("GetCorner", &GfRange2d::GetCorner)
        .def("GetQuadrant", &GfRange2d::GetQuadrant)
        .def_readonly("unitSquare", &GfRange2d::UnitSquare)
        
        ;
    boost::python::to_python_converter<std::vector<GfRange2d>,
        TfPySequenceToPython<std::vector<GfRange2d> > >();
    
}
