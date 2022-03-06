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
// wrapVec.template.cpp file to make changes.

#include "pxr/pxr.h"
#include "pxr/base/gf/vec4d.h"

#include "pxr/base/gf/pyBufferUtils.h"

#include "pxr/base/tf/py3Compat.h"
#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/stringUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

// Include headers for other vec types to support wrapping conversions and
// operators.
#include "pxr/base/gf/vec4f.h"
#include "pxr/base/gf/vec4h.h"
#include "pxr/base/gf/vec4i.h"

#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/slice.hpp>

#include <boost/functional/hash.hpp>

#include <string>



PXR_NAMESPACE_USING_DIRECTIVE

namespace pxrBaseGfWrapVec4d {

////////////////////////////////////////////////////////////////////////
// Python buffer protocol support.

#if PY_MAJOR_VERSION == 2
// Python's getreadbuf interface function.
static Py_ssize_t
getreadbuf(PyObject *self, Py_ssize_t segment, void **ptrptr) {
    if (segment != 0) {
        // Always one-segment.
        PyErr_SetString(PyExc_ValueError, "accessed non-existent segment");
        return -1;
    }
    GfVec4d &vec = boost::python::extract<GfVec4d &>(self);
    *ptrptr = static_cast<void *>(vec.data());
    // Return size in bytes.
    return sizeof(GfVec4d);
}

// Python's getwritebuf interface function.
static Py_ssize_t
getwritebuf(PyObject *self, Py_ssize_t segment, void **ptrptr) {
    PyErr_SetString(PyExc_ValueError, "writable buffers supported only with "
                    "new-style buffer protocol.");
    return -1;
}

// Python's getsegcount interface function.
static Py_ssize_t
getsegcount(PyObject *self, Py_ssize_t *lenp) {
    if (lenp)
        *lenp = sizeof(GfVec4d);
    return 1; // Always one contiguous segment.
}

// Python's getcharbuf interface function.
static Py_ssize_t
getcharbuf(PyObject *self, Py_ssize_t segment, const char **ptrptr) {
    PyErr_SetString(PyExc_ValueError, "cannot treat binary data as text");
    return -1;
}
#endif

// Python's getbuffer interface function.
static int
getbuffer(PyObject *self, Py_buffer *view, int flags) {
    if (view == NULL) {
        PyErr_SetString(PyExc_ValueError, "NULL view in getbuffer");
        return -1;
    }

    // We don't support fortran order.
    if ((flags & PyBUF_F_CONTIGUOUS) == PyBUF_F_CONTIGUOUS) {
        PyErr_SetString(PyExc_ValueError, "Fortran contiguity unsupported");
        return -1;
    }

    GfVec4d &vec = boost::python::extract<GfVec4d &>(self);

    view->obj = self;
    view->buf = static_cast<void *>(vec.data());
    view->len = sizeof(GfVec4d);
    view->readonly = 0;
    view->itemsize = sizeof(double);
    if ((flags & PyBUF_FORMAT) == PyBUF_FORMAT) {
        view->format = Gf_GetPyBufferFmtFor<double>();
    } else {
        view->format = NULL;
    }
    if ((flags & PyBUF_ND) == PyBUF_ND) {
        view->ndim = 1;
        static Py_ssize_t shape = 4;
        view->shape = &shape;
    } else {
        view->ndim = 0;
        view->shape = NULL;
    }
    if ((flags & PyBUF_STRIDES) == PyBUF_STRIDES) {
        static Py_ssize_t strides = sizeof(double);
        view->strides = &strides;
    } else {
        view->strides = NULL;
    }
    view->suboffsets = NULL;
    view->internal = NULL;

    Py_INCREF(self); // need to retain a reference to self.
    return 0;
}

// This structure serves to instantiate a PyBufferProcs instance with pointers
// to the right buffer protocol functions.
static PyBufferProcs bufferProcs = {
#if PY_MAJOR_VERSION == 2
    (readbufferproc) getreadbuf,   /*bf_getreadbuffer*/
    (writebufferproc) getwritebuf, /*bf_getwritebuffer*/
    (segcountproc) getsegcount,    /*bf_getsegcount*/
    (charbufferproc) getcharbuf,   /*bf_getcharbuffer*/
#endif
    (getbufferproc) getbuffer,
    (releasebufferproc) 0,
};

// End python buffer protocol support.
////////////////////////////////////////////////////////////////////////


static std::string __repr__(GfVec4d const &self) {
    std::string elems;
    for (size_t i = 0; i < self.dimension; ++i) 
        elems += (i ? ", " : "") + TfPyRepr(self[i]);

    return TF_PY_REPR_PREFIX + "Vec4d(" + elems + ")";
}

static size_t __hash__(GfVec4d const &self) {
    return boost::hash<GfVec4d>()(self);
}


BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(VecGetNormalized_overloads,
                                       GetNormalized, 0, 1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(VecNormalize_overloads, Normalize, 0, 1);
BOOST_PYTHON_FUNCTION_OVERLOADS(GetNormalized_overloads,
                                GfGetNormalized, 1, 2);

static double
NormalizeHelper(GfVec4d *vec, double eps = GF_MIN_VECTOR_LENGTH)
{
    return GfNormalize(vec, eps);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(Normalize_overloads, NormalizeHelper, 1, 2);

 

 
static int
normalizeIndex(int index) {
    return TfPyNormalizeIndex(index, 4, true /*throw error*/);
}

static int __len__(const GfVec4d &self) { return 4; }

// Implements __getitem__ for a single index
static double __getitem__(const GfVec4d &self, int index) {
    return self[normalizeIndex(index)];
}

// Implements __getitem__ for a slice
static boost::python::list __getslice__(const GfVec4d &self, boost::python::slice indices) {
    boost::python::list result;

    const double* begin = self.data();
    const double* end = begin + 4;

    boost::python::slice::range<const double*> bounds;
    try {
        // This appears to be a typo in the boost headers.  The method
        // name should be "get_indices".
        //
        bounds = indices.get_indicies<>(begin, end);
    } catch (std::invalid_argument) {
        return result;
    }

    while (bounds.start != bounds.stop) {
        result.append(*bounds.start);
        bounds.start += bounds.step;
    }
    // Unlike STL ranges, bounds represents a *closed* interval.  This
    // means that we must append exactly one more item at the end of
    // the list.
    //
    result.append(*bounds.start);

    return result;
}

static void __setitem__(GfVec4d &self, int index, double value) {
    self[normalizeIndex(index)] = value;
}

// Handles refcounting & extraction for PySequence_GetItem.
static double _SequenceGetItem(PyObject *seq, Py_ssize_t i) {
    boost::python::handle<> h(PySequence_GetItem(seq, i));
    return boost::python::extract<double>(boost::python::object(h));
}

static bool _SequenceCheckItem(PyObject *seq, Py_ssize_t i) {
    boost::python::handle<> h(PySequence_GetItem(seq, i));
    boost::python::extract<double> e((boost::python::object(h)));
    return e.check();
}

static void __setslice__(GfVec4d &self, boost::python::slice indices, boost::python::object values) {
    // Verify our arguments
    //
    PyObject* valuesObj = values.ptr();

    if (!PySequence_Check(valuesObj)) {
        TfPyThrowTypeError("value must be a sequence");
    }

    double* begin = self.data();
    double* end = begin + 4;

    Py_ssize_t sliceLength = -1;

    boost::python::slice::range<double*> bounds;

    // Convince g++ that we're not using uninitialized values.
    //
    bounds.start = 0;
    bounds.stop  = 0;
    bounds.step  = 0;

    try {
        // This appears to be a typo in the boost headers.  The method
        // name should be "get_indices".
        //
        bounds = indices.get_indicies<>(begin, end);
    } catch (std::invalid_argument) {
        sliceLength = 0;
    }

    // If sliceLength was not set in the exception handling code above,
    // figure out how long it really is.
    //
    if (sliceLength == -1) {
        sliceLength = ((bounds.stop - bounds.start) / bounds.step) + 1;
    }

    if (PySequence_Length(valuesObj) != sliceLength) {
        TfPyThrowValueError(
            TfStringPrintf(
                "attempt to assign sequence of size %zd to slice of size %zd",
                PySequence_Length(valuesObj), sliceLength));
    }

    // Short circuit for empty slices
    //
    if (sliceLength == 0) {
        return;
    }

    // Make sure that all items can be extracted before changing the GfVec4d.
    //
    for (Py_ssize_t i = 0; i < sliceLength; ++i) {
        // This will throw a TypeError if any of the items cannot be converted.
        _SequenceGetItem(valuesObj, i);
    }

    for (Py_ssize_t i = 0; i < sliceLength; ++i) {
        *bounds.start = _SequenceGetItem(valuesObj, i);
        bounds.start += bounds.step;
    }
}

static bool __contains__(const GfVec4d &self, double value) {
    for (size_t i = 0; i < 4; ++i) {
        if (self[i] == value)
            return true;
    }
    return false;
}

#if PY_MAJOR_VERSION == 2
static GfVec4d __truediv__(const GfVec4d &self, double value)
{
    return self / value;
}

static GfVec4d __itruediv__(GfVec4d &self, double value)
{
    return self /= value;
}
#endif

template <class V>
static V *__init__() {
    // Default contstructor zero-initializes from python.
    return new V(0);
}

struct FromPythonTuple {
    FromPythonTuple() {
        boost::python::converter::registry::
            push_back(&_convertible, &_construct,
                      boost::python::type_id<GfVec4d>());
    }

  private:
    
    static void *_convertible(PyObject *obj_ptr) {
        // If this object is a GfVec already, disregard.
        if (PyObject_HasAttrString(obj_ptr, "__isGfVec"))
            return 0;

        typedef double Scalar;

        // XXX: Would like to allow general sequences, but currently clients
        // depend on this behavior.
        if ((PyTuple_Check(obj_ptr) || PyList_Check(obj_ptr)) &&
            PySequence_Size(obj_ptr) == 4 &&
            _SequenceCheckItem(obj_ptr, 0) &&
            _SequenceCheckItem(obj_ptr, 1) &&
            _SequenceCheckItem(obj_ptr, 2) &&
            _SequenceCheckItem(obj_ptr, 3)) {
            return obj_ptr;
        }
        return 0;
    }

    static void _construct(PyObject *obj_ptr, boost::python::converter::
                           rvalue_from_python_stage1_data *data) {
        typedef double Scalar;
        void *storage = ((boost::python::converter::rvalue_from_python_storage<GfVec4d>*)data)
	    ->storage.bytes;
        new (storage)
	    GfVec4d(
                _SequenceGetItem(obj_ptr, 0),
                _SequenceGetItem(obj_ptr, 1),
                _SequenceGetItem(obj_ptr, 2),
                _SequenceGetItem(obj_ptr, 3));
        data->convertible = storage;
    }
};

// This adds support for python's builtin pickling library
// This is used by our Shake plugins which need to pickle entire classes
// (including code), which we don't support in pxml.
struct PickleSuite : boost::python::pickle_suite
{
    static boost::python::tuple getinitargs(const GfVec4d &v) {
        return boost::python::make_tuple(v[0], v[1], v[2], v[3]);
    }
};

} // anonymous namespace 

void wrapVec4d()
{
    typedef GfVec4d Vec;
    typedef double Scalar;

    static const size_t _dimension = 4;
    static const bool _true = true;
    
    boost::python::def("Dot", (Scalar (*)( const Vec &, const Vec &))GfDot);
    
    boost::python::def("CompDiv", (Vec (*)(const Vec &v1, const Vec&v2))GfCompDiv);
    boost::python::def("CompMult", (Vec (*)(const Vec &v1, const Vec&v2))GfCompMult);
    boost::python::def("GetLength", (Scalar (*)(const Vec &v))GfGetLength);
    boost::python::def("GetNormalized", (Vec (*)(const Vec &v, Scalar eps))
        GfGetNormalized, pxrBaseGfWrapVec4d::GetNormalized_overloads());
    boost::python::def("GetProjection", (Vec (*)(const Vec &a, const Vec &b))
        GfGetProjection);
    boost::python::def("GetComplement", (Vec (*)(const Vec &a, const Vec &b))
        GfGetComplement);
    boost::python::def("IsClose", (bool (*)(const Vec &v1, const Vec &v2, double))
        GfIsClose);
    boost::python::def("Normalize", pxrBaseGfWrapVec4d::NormalizeHelper, pxrBaseGfWrapVec4d::Normalize_overloads());
        

 
    boost::python::class_<GfVec4d> cls("Vec4d", boost::python::no_init);
    cls
        .def("__init__", boost::python::make_constructor(pxrBaseGfWrapVec4d::__init__<Vec>))

        // A tag indicating that this is a GfVec class, for internal use.
        .def_readonly("__isGfVec", _true)

        .def_pickle(pxrBaseGfWrapVec4d::PickleSuite())

        // Conversion from other vec types.
        .def(boost::python::init<GfVec4f>())
        .def(boost::python::init<GfVec4h>())
        .def(boost::python::init<GfVec4i>())
        
        .def(boost::python::init<Vec>())
        .def(boost::python::init<Scalar>())
        .def(boost::python::init<Scalar, Scalar, Scalar, Scalar>())

        .def(TfTypePythonClass())

        .def("__len__", pxrBaseGfWrapVec4d::__len__ )
        .def("__getitem__", pxrBaseGfWrapVec4d::__getitem__ )
        .def("__getitem__", pxrBaseGfWrapVec4d::__getslice__ )
        .def("__setitem__", pxrBaseGfWrapVec4d::__setitem__ )
        .def("__setitem__", pxrBaseGfWrapVec4d::__setslice__ )
        .def("__contains__", pxrBaseGfWrapVec4d::__contains__ )

        .def_readonly("dimension", _dimension)
        
        // Comparison to other vec types.
        .def( boost::python::self == GfVec4f() )
        .def( boost::python::self != GfVec4f() )
        .def( boost::python::self == GfVec4h() )
        .def( boost::python::self != GfVec4h() )
        .def( boost::python::self == GfVec4i() )
        .def( boost::python::self != GfVec4i() )

        .def(boost::python::self == boost::python::self)
        .def(boost::python::self != boost::python::self)
        .def(boost::python::self += boost::python::self)
        .def(boost::python::self -= boost::python::self)
        .def(boost::python::self *= double())
        .def(boost::python::self * double())
        .def(double() * boost::python::self)
        .def(boost::python::self /= double())
        .def(boost::python::self / double())
        .def(-boost::python::self)
        .def(boost::python::self + boost::python::self)
        .def(boost::python::self - boost::python::self)
        .def(boost::python::self * boost::python::self)
        .def(boost::python::self_ns::str(boost::python::self))

#if PY_MAJOR_VERSION == 2
        // Needed only to support "from __future__ import division" in
        // python 2. In python 3 builds boost::python adds this for us.
        .def("__truediv__", pxrBaseGfWrapVec4d::__truediv__ )
        .def("__itruediv__", pxrBaseGfWrapVec4d::__itruediv__ )
#endif

        .def("Axis", &Vec::Axis).staticmethod("Axis")

        .def("XAxis", &Vec::XAxis).staticmethod("XAxis")
        .def("YAxis", &Vec::YAxis).staticmethod("YAxis")
        .def("ZAxis", &Vec::ZAxis).staticmethod("ZAxis")
        .def("WAxis", &Vec::WAxis).staticmethod("WAxis")

        .def("GetDot", (Scalar (*)(const Vec &, const Vec &))GfDot)
        
        .def("GetComplement", &Vec::GetComplement)
        .def("GetLength", &Vec::GetLength)
        .def("GetNormalized", &Vec::GetNormalized, pxrBaseGfWrapVec4d::VecGetNormalized_overloads())
        .def("GetProjection", &Vec::GetProjection)
        .def("Normalize", &Vec::Normalize, pxrBaseGfWrapVec4d::VecNormalize_overloads())

        .def("__repr__", pxrBaseGfWrapVec4d::__repr__)
        .def("__hash__", pxrBaseGfWrapVec4d::__hash__)
        ;
    boost::python::to_python_converter<std::vector<GfVec4d>,
        TfPySequenceToPython<std::vector<GfVec4d> > >();

    // Install buffer protocol: set the tp_as_buffer slot to point to a
    // structure of function pointers that implement the buffer protocol for
    // this type, and set the type flags to indicate that this type supports the
    // buffer protocol.
    auto *typeObj = reinterpret_cast<PyTypeObject *>(cls.ptr());
    typeObj->tp_as_buffer = &pxrBaseGfWrapVec4d::bufferProcs;
    typeObj->tp_flags |= (TfPy_TPFLAGS_HAVE_NEWBUFFER |
                          TfPy_TPFLAGS_HAVE_GETCHARBUFFER);

    // Allow appropriate tuples to be passed where Vecs are expected.
    pxrBaseGfWrapVec4d::FromPythonTuple();

    // Allow conversion of lists of GfVec4d to std::vector<GfVec4d>
    TfPyContainerConversions::from_python_sequence<
        std::vector<GfVec4d>,
        TfPyContainerConversions::variable_capacity_policy >();
}
