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

#include "pxr/base/tf/pyObjWrapper.h"

#include <boost/python.hpp>


PXR_NAMESPACE_USING_DIRECTIVE

namespace pxrBaseTfWrapPyObjWrapper {

struct Tf_PyObjWrapperFromPython {
    Tf_PyObjWrapperFromPython() {
        boost::python::converter::registry::
            push_back(&_convertible, &_construct,
                      boost::python::type_id<TfPyObjWrapper>());
    }

private:

    static void*
    _convertible(PyObject *o) {
        // Can always put a python object in a TfPyObjWrapper.
        return o;
    }

    static void
    _construct(PyObject *obj_ptr,
               boost::python::converter::rvalue_from_python_stage1_data *data) {
        void *storage =
            ((boost::python::converter::rvalue_from_python_storage<TfPyObjWrapper>*)data)
            ->storage.bytes;
        // Make a TfPyObjWrapper holding the Python object.
        new (storage) TfPyObjWrapper(boost::python::object(boost::python::borrowed(obj_ptr)));
        data->convertible = storage;
    }
};

struct Tf_PyObjWrapperToPython {
    static PyObject *
    convert(TfPyObjWrapper const &val) {
        return boost::python::incref(val.Get().ptr());
    }
};

static TfPyObjWrapper
_RoundTripWrapperTest(TfPyObjWrapper const &wrapper)
{
    return wrapper;
}

static TfPyObjWrapper
_RoundTripWrapperCallTest(TfPyObjWrapper const &wrapper)
{
    return wrapper();
}

static TfPyObjWrapper
_RoundTripWrapperIndexTest(TfPyObjWrapper const &wrapper, int index)
{
    return boost::python::object(wrapper[index]);
}

} // anonymous namespace 

void wrapPyObjWrapper()
{
    boost::python::to_python_converter<TfPyObjWrapper, pxrBaseTfWrapPyObjWrapper::Tf_PyObjWrapperToPython>();
    pxrBaseTfWrapPyObjWrapper::Tf_PyObjWrapperFromPython();

    boost::python::def("_RoundTripWrapperTest", pxrBaseTfWrapPyObjWrapper::_RoundTripWrapperTest);
    boost::python::def("_RoundTripWrapperCallTest", pxrBaseTfWrapPyObjWrapper::_RoundTripWrapperCallTest);
    boost::python::def("_RoundTripWrapperIndexTest", pxrBaseTfWrapPyObjWrapper::_RoundTripWrapperIndexTest);
}
