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
// This file is generated by a script.  Do not edit directly.  Edit the
// wrapMatrix2.template.cpp file to make changes.


#include "pxr/pxr.h"
#include "pxr/base/gf/matrix2d.h"
#include "pxr/base/gf/matrix2f.h"

#include "pxr/base/gf/pyBufferUtils.h"


#include "pxr/base/tf/py3Compat.h"
#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/detail/api_placeholder.hpp>
#include <boost/python/errors.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/return_arg.hpp>
#include <boost/python/tuple.hpp>

#include <string>
#include <vector>


PXR_NAMESPACE_USING_DIRECTIVE

namespace {

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
    GfMatrix2f &mat = extract<GfMatrix2f &>(self);
    *ptrptr = static_cast<void *>(mat.GetArray());
    // Return size in bytes.
    return sizeof(GfMatrix2f);
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
        *lenp = sizeof(GfMatrix2f);
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

    GfMatrix2f &mat = boost::python::extract<GfMatrix2f &>(self);

    view->obj = self;
    view->buf = static_cast<void *>(mat.GetArray());
    view->len = sizeof(GfMatrix2f);
    view->readonly = 0;
    view->itemsize = sizeof(float);
    if ((flags & PyBUF_FORMAT) == PyBUF_FORMAT) {
        view->format = Gf_GetPyBufferFmtFor<float>();
    } else {
        view->format = NULL;
    }
    if ((flags & PyBUF_ND) == PyBUF_ND) {
        view->ndim = 2;
        static Py_ssize_t shape[] = { 2, 2 };
        view->shape = shape;
    } else {
        view->ndim = 0;
        view->shape = NULL;
    }
    if ((flags & PyBUF_STRIDES) == PyBUF_STRIDES) {
        static Py_ssize_t strides[] = {
            2 * sizeof(float), sizeof(float) };
        view->strides = strides;
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

static std::string _Repr(GfMatrix2f const &self) {
    static char newline[] = ",\n            ";
    return TF_PY_REPR_PREFIX + "Matrix2f(" +
        TfPyRepr(self[0][0]) + ", " + TfPyRepr(self[0][1]) + newline +
        TfPyRepr(self[1][0]) + ", " + TfPyRepr(self[1][1]) + ")";
}

static GfMatrix2f GetInverseWrapper( const GfMatrix2f &self ) {
    return self.GetInverse();
}

static void
throwIndexErr( const char *msg )
{
    PyErr_SetString(PyExc_IndexError, msg);
    boost::python::throw_error_already_set();
}    

static int
normalizeIndex(int index) {
    return TfPyNormalizeIndex(index, 2, true /*throw error*/);
}

// Return number of rows
static int __len__(GfMatrix2f const &self) {
    return 2;
}

static float __getitem__float(GfMatrix2f const &self, boost::python::tuple index) {
    int i1=0, i2=0;
    if (boost::python::len(index) == 2) {
        i1 = normalizeIndex(boost::python::extract<int>(index[0]));
        i2 = normalizeIndex(boost::python::extract<int>(index[1]));
    } else
        throwIndexErr("Index has incorrect size.");

    return self[i1][i2];
}

static GfVec2f __getitem__vector(GfMatrix2f const &self, int index) {
    return GfVec2f(self[normalizeIndex(index)]);
}

static void __setitem__float(GfMatrix2f &self, boost::python::tuple index, float value) {
    int i1=0, i2=0;
    if (boost::python::len(index) == 2) {
        i1 = normalizeIndex(boost::python::extract<int>(index[0]));
        i2 = normalizeIndex(boost::python::extract<int>(index[1]));
    } else
        throwIndexErr("Index has incorrect size.");

    self[i1][i2] = value;
}

static void __setitem__vector( GfMatrix2f &self, int index, GfVec2f value ) {
    int ni = normalizeIndex(index);
    self[ni][0] = value[0];
    self[ni][1] = value[1];
}

static bool __contains__float( const GfMatrix2f &self, float value ) {
    for( int i = 0; i < 2; ++i )
        for( int j = 0; j < 2; ++j )
            if( self[i][j] == value )
                return true;
    return false;
}

// Check rows against GfVec
static bool __contains__vector( const GfMatrix2f &self, GfVec2f value ) {
    for( int i = 0; i < 2; ++i )
	if( self.GetRow(i) == value )
	    return true;
    return false;
}

#if PY_MAJOR_VERSION == 2
static GfMatrix2f __truediv__(const GfMatrix2f &self, GfMatrix2f value)
{
    return self / value;
}
#endif

static GfMatrix2f *__init__() {
    // Default constructor produces identity from python.
    return new GfMatrix2f(1);
}


// This adds support for python's builtin pickling library
// This is used by our Shake plugins which need to pickle entire classes
// (including code), which we don't support in pxml.
struct GfMatrix2f_Pickle_Suite : boost::python::pickle_suite
{
    static boost::python::tuple getinitargs(const GfMatrix2f &m)
    {
        return boost::python::make_tuple(
            m[0][0], m[0][1], 
            m[1][0], m[1][1]);
    }
};

static size_t __hash__(GfMatrix2f const &m) { return hash_value(m); }

static boost::python::tuple get_dimension()
{
    // At one time this was a constant static tuple we returned for
    // dimension. With boost building for python 3 that results in
    // a segfault at shutdown. Building for python 2 with a static
    // tuple returned here seems to work fine.
    //
    // It seems likely that this has to do with the order of
    // destruction of these objects when deinitializing, but we did
    // not dig deeply into this difference.
    return boost::python::make_tuple(2, 2);
}

} // anonymous namespace 

void wrapMatrix2f()
{    
    typedef GfMatrix2f This;

    boost::python::def("IsClose", (bool (*)(const GfMatrix2f &m1, const GfMatrix2f &m2, double))
        GfIsClose);
    
    boost::python::class_<This> cls( "Matrix2f", boost::python::no_init);
    cls
        .def_pickle(GfMatrix2f_Pickle_Suite())
	.def("__init__", boost::python::make_constructor(__init__))
        .def(boost::python::init< const GfMatrix2d & >())
        .def(boost::python::init< const GfMatrix2f & >())
        .def(boost::python::init< int >())
        .def(boost::python::init< float >())
        .def(boost::python::init<
             float, float, 
             float, float 
             >())
        .def(boost::python::init< const GfVec2f & >())
        .def(boost::python::init< const std::vector< std::vector<float> >& >())
        .def(boost::python::init< const std::vector< std::vector<double> >& >())

        .def( TfTypePythonClass() )

        .add_static_property("dimension", get_dimension)
        .def( "__len__", __len__, "Return number of rows" )

        .def( "__getitem__", __getitem__float )
        .def( "__getitem__", __getitem__vector )
        .def( "__setitem__", __setitem__float )
        .def( "__setitem__", __setitem__vector )
        .def( "__contains__", __contains__float )
        .def( "__contains__", __contains__vector, "Check rows against GfVec"  )

        .def("Set", (This &(This::*)(float, float, 
                                     float, float))&This::Set,
             boost::python::return_self<>())
        
        .def("SetIdentity", &This::SetIdentity, boost::python::return_self<>())
        .def("SetZero", &This::SetZero, boost::python::return_self<>())

        .def("SetDiagonal", 
             (This & (This::*)(float))&This::SetDiagonal, 
             boost::python::return_self<>())
        .def("SetDiagonal", 
             (This & (This::*)(const GfVec2f &))&This::SetDiagonal, 
             boost::python::return_self<>())

        .def("SetRow", &This::SetRow)
        .def("SetColumn", &This::SetColumn)
        .def("GetRow", &This::GetRow)
        .def("GetColumn", &This::GetColumn)

        .def("GetTranspose", &This::GetTranspose)
        .def("GetInverse", GetInverseWrapper)

        .def("GetDeterminant", &This::GetDeterminant)
        
        .def( boost::python::self_ns::str(boost::python::self) )
        .def( boost::python::self == boost::python::self )
        .def( boost::python::self == GfMatrix2d() )
        .def( boost::python::self != boost::python::self )
        .def( boost::python::self != GfMatrix2d() )
        .def( boost::python::self *= boost::python::self )
        .def( boost::python::self * boost::python::self )
        .def( boost::python::self *= double() )
        .def( boost::python::self * double() )
        .def( double() * boost::python::self )
        .def( boost::python::self += boost::python::self )
        .def( boost::python::self + boost::python::self )
        .def( boost::python::self -= boost::python::self )
        .def( boost::python::self - boost::python::self )
        .def( -boost::python::self )
        .def( boost::python::self / boost::python::self )
        .def( boost::python::self * GfVec2f() )
        .def( GfVec2f() * boost::python::self )

#if PY_MAJOR_VERSION == 2
        // Needed only to support "from __future__ import division" in
        // python 2. In python 3 builds boost::python adds this for us.
        .def("__truediv__", __truediv__ )
#endif

        .def("__repr__", _Repr)
        .def("__hash__", __hash__)

        ;
    boost::python::to_python_converter<std::vector<This>,
        TfPySequenceToPython<std::vector<This> > >();
    
    // Install buffer protocol: set the tp_as_buffer slot to point to a
    // structure of function pointers that implement the buffer protocol for
    // this type, and set the type flags to indicate that this type supports the
    // buffer protocol.
    auto *typeObj = reinterpret_cast<PyTypeObject *>(cls.ptr());
    typeObj->tp_as_buffer = &bufferProcs;
    typeObj->tp_flags |= (TfPy_TPFLAGS_HAVE_NEWBUFFER |
                          TfPy_TPFLAGS_HAVE_GETCHARBUFFER);
}
