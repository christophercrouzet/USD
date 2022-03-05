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
#include "pxr/usd/usdSkel/skinningQuery.h"

#include "pxr/base/gf/interval.h"
#include "pxr/base/gf/matrix4d.h"
#include "pxr/base/gf/matrix4f.h"

#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyPtrHelpers.h"
#include "pxr/base/tf/pyResultConversions.h"
#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

#include "pxr/usd/usd/pyConversions.h"
#include "pxr/usd/usdGeom/boundable.h"

#include <boost/python.hpp>



PXR_NAMESPACE_USING_DIRECTIVE


namespace {


boost::python::object
_ComputeJointInfluences(const UsdSkelSkinningQuery& self, UsdTimeCode time)
{
    VtIntArray indices;
    VtFloatArray weights;
    if (self.ComputeJointInfluences(&indices, &weights, time)) {
        return boost::python::make_tuple(indices, weights);
    }
    return boost::python::object();
}


boost::python::object
_ComputeVaryingJointInfluences(const UsdSkelSkinningQuery& self,
                               size_t numPoints,
                               UsdTimeCode time)
{
    VtIntArray indices;
    VtFloatArray weights;
    if (self.ComputeVaryingJointInfluences(numPoints, &indices,
                                          &weights, time)) {
        return boost::python::make_tuple(indices, weights);
    }
    return boost::python::object();
}


std::vector<double>
_GetTimeSamples(const UsdSkelSkinningQuery& self)
{
    std::vector<double> times;
    self.GetTimeSamples(&times);
    return times;
}


std::vector<double>
_GetTimeSamplesInInterval(const UsdSkelSkinningQuery& self,
                          const GfInterval& interval)
{
    std::vector<double> times;
    self.GetTimeSamplesInInterval(interval, &times);
    return times;
}


boost::python::object
_GetJointOrder(const UsdSkelSkinningQuery& self)
{
    VtTokenArray jointOrder;
    if (self.GetJointOrder(&jointOrder))
        return boost::python::object(jointOrder);
    return {};
}


boost::python::object
_GetBlendShapeOrder(const UsdSkelSkinningQuery& self)
{
    VtTokenArray blendShapeOrder;
    if (self.GetBlendShapeOrder(&blendShapeOrder))
        return boost::python::object(blendShapeOrder);
    return {};
}

template <typename Matrix4>
bool
_ComputeSkinnedPoints(const UsdSkelSkinningQuery& self,
                      const VtArray<Matrix4>& xforms,
                      VtVec3fArray& points,
                      UsdTimeCode time)
{
    return self.ComputeSkinnedPoints(xforms, &points, time);
}


template <typename Matrix4>
Matrix4
_ComputeSkinnedTransform(const UsdSkelSkinningQuery& self,
                         const VtArray<Matrix4>& xforms,
                         UsdTimeCode time)
{
    Matrix4 xform;
    self.ComputeSkinnedTransform(xforms, &xform, time);
    return xform;
}


} // namespace


void wrapUsdSkelSkinningQuery()
{
    using This = UsdSkelSkinningQuery;

    boost::python::class_<This>("SkinningQuery")

        .def(!boost::python::self)

        .def("__str__", &This::GetDescription)

        .def("GetPrim", &This::GetPrim,
             boost::python::return_value_policy<boost::python::return_by_value>())

        .def("HasJointInfluences", &This::HasJointInfluences)

        .def("HasBlendShapes", &This::HasBlendShapes)
        
        .def("GetNumInfluencesPerComponent",
             &This::GetNumInfluencesPerComponent)

        .def("GetInterpolation", &This::GetInterpolation,
             boost::python::return_value_policy<boost::python::return_by_value>())

        .def("IsRigidlyDeformed", &This::IsRigidlyDeformed)

        .def("GetGeomBindTransformAttr", &This::GetGeomBindTransformAttr,
             boost::python::return_value_policy<boost::python::return_by_value>())

        .def("GetJointIndicesPrimvar", &This::GetJointIndicesPrimvar,
             boost::python::return_value_policy<boost::python::return_by_value>())

        .def("GetJointWeightsPrimvar", &This::GetJointWeightsPrimvar,
             boost::python::return_value_policy<boost::python::return_by_value>())

        .def("GetBlendShapesAttr", &This::GetBlendShapesAttr,
             boost::python::return_value_policy<boost::python::return_by_value>())

        .def("GetBlendShapeTargetsRel", &This::GetBlendShapeTargetsRel,
             boost::python::return_value_policy<boost::python::return_by_value>())

        // deprecated
        .def("GetMapper", &This::GetMapper,
             boost::python::return_value_policy<boost::python::return_by_value>())

        .def("GetJointMapper", &This::GetJointMapper,
             boost::python::return_value_policy<boost::python::return_by_value>())

        .def("GetBlendShapeMapper", &This::GetBlendShapeMapper,
             boost::python::return_value_policy<boost::python::return_by_value>())

        .def("GetJointOrder", &_GetJointOrder)

        .def("GetBlendShapeOrder", &_GetBlendShapeOrder)

        .def("GetTimeSamples", &_GetTimeSamples)

        .def("GetTimeSamplesInInterval", &_GetTimeSamplesInInterval)

        .def("ComputeJointInfluences", &_ComputeJointInfluences,
             (boost::python::arg("time")=UsdTimeCode::Default()))

        .def("ComputeVaryingJointInfluences", &_ComputeVaryingJointInfluences,
             (boost::python::arg("numPoints"), boost::python::arg("time")=UsdTimeCode::Default()))

        .def("ComputeSkinnedPoints", &_ComputeSkinnedPoints<GfMatrix4d>,
             (boost::python::arg("xforms"), boost::python::arg("points"),
              boost::python::arg("time")=UsdTimeCode::Default()))

        .def("ComputeSkinnedPoints", &_ComputeSkinnedPoints<GfMatrix4f>,
             (boost::python::arg("xforms"), boost::python::arg("points"),
              boost::python::arg("time")=UsdTimeCode::Default()))

        .def("ComputeSkinnedTransform", &_ComputeSkinnedTransform<GfMatrix4d>,
             (boost::python::arg("xforms"),
              boost::python::arg("time")=UsdTimeCode::Default()))

        .def("ComputeSkinnedTransform", &_ComputeSkinnedTransform<GfMatrix4f>,
             (boost::python::arg("xforms"),
              boost::python::arg("time")=UsdTimeCode::Default()))

        .def("ComputeExtentsPadding",
             static_cast<float (UsdSkelSkinningQuery::*)(
                 const VtMatrix4dArray&,
                 const UsdGeomBoundable&) const>(
                     &This::ComputeExtentsPadding),
             (boost::python::arg("skelRestXforms"),
              boost::python::arg("boundable"),
              boost::python::arg("time")=UsdTimeCode::Default()))

        .def("ComputeExtentsPadding",
             static_cast<float (UsdSkelSkinningQuery::*)(
                 const VtMatrix4fArray&,
                 const UsdGeomBoundable&) const>(
                     &This::ComputeExtentsPadding),
             (boost::python::arg("skelRestXforms"),
              boost::python::arg("boundable"),
              boost::python::arg("time")=UsdTimeCode::Default()))

        .def("GetGeomBindTransform", &This::GetGeomBindTransform,
             (boost::python::arg("time")=UsdTimeCode::Default()))
        ;
}
