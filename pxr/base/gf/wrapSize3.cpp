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
#include "pxr/base/gf/size3.h"

#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"
#include "pxr/base/tf/pyContainerConversions.h"

#include <boost/python/class.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/return_arg.hpp>

#include <string>



PXR_NAMESPACE_USING_DIRECTIVE

namespace pxrBaseGfWrapSize3 {

static int
normalizeIndex(int index) {
    return TfPyNormalizeIndex(index, 3, true /*throw error*/);
}

static int __len__(const GfSize3 &self) {
    return 3;
}

static size_t __getitem__(const GfSize3 &self, int index) {
    index = normalizeIndex(index);
    return self[index];
}

static void __setitem__(GfSize3 &self, int index, size_t value) {
    index = normalizeIndex(index);
    self[index] = value;
}
    
static bool __contains__(const GfSize3 &self, size_t value) {
    if (self[0] == value || self[1] == value || self[2] == value)
        return true;
    return false;
}

#if PY_MAJOR_VERSION == 2
static GfSize3 __truediv__(const GfSize3 &self, size_t value)
{
    return self / value;
}

static GfSize3 __itruediv__(GfSize3 &self, size_t value)
{
    return self /= value;
}
#endif

static std::string _Repr(GfSize3 const &self) {
    return TF_PY_REPR_PREFIX + "Size3(" + TfPyRepr(self[0]) + ", " +
        TfPyRepr(self[1]) + ", " + TfPyRepr(self[2]) + ")";
}

} // anonymous namespace 

void wrapSize3()
{    
    typedef GfSize3 This;

    static const int dimension = 3;

    boost::python::class_<This>( "Size3", "A 3D size class", boost::python::init<>() )
        .def(boost::python::init<const This &>())
        .def(boost::python::init<const GfVec3i &>())
        .def(boost::python::init<size_t, size_t, size_t>())

        .def( TfTypePythonClass() )

        .def("Set", (GfSize3 &(This::*)(size_t, size_t, size_t))&This::Set, boost::python::return_self<>())

        .def_readonly("dimension", dimension)

        .def("__len__", pxrBaseGfWrapSize3::__len__)
        .def("__getitem__", pxrBaseGfWrapSize3::__getitem__)
        .def("__setitem__", pxrBaseGfWrapSize3::__setitem__)
        .def("__contains__", pxrBaseGfWrapSize3::__contains__)

        .def( boost::python::self_ns::str(boost::python::self) )
        .def( boost::python::self == boost::python::self )
        .def( boost::python::self != boost::python::self )

        .def( boost::python::self += boost::python::self )
        .def( boost::python::self -= boost::python::self )
        .def( boost::python::self *= size_t() )
        .def( boost::python::self /= size_t() )
        .def( boost::python::self + boost::python::self )
        .def( boost::python::self - boost::python::self )
        .def( boost::python::self * boost::python::self )
        .def( size_t() * boost::python::self )
        .def( boost::python::self * size_t() )
        .def( boost::python::self / size_t() )
        
#if PY_MAJOR_VERSION == 2
        // Needed only to support "from __future__ import division" in
        // python 2. In python 3 builds boost::python adds this for us.
        .def("__truediv__", __truediv__ )
        .def("__itruediv__", __itruediv__ )
#endif

        .def("__repr__", pxrBaseGfWrapSize3::_Repr)

        ;
    boost::python::to_python_converter<std::vector<This>,
        TfPySequenceToPython<std::vector<This> > >();


    // conversion operator
    boost::python::implicitly_convertible<This, GfVec3i>();
    
}
