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
// quat.template.cpp file to make changes.

#include "pxr/pxr.h"
#include "pxr/base/gf/quath.h"
#include "pxr/base/gf/ostreamHelpers.h"
#include "pxr/base/gf/vec4h.h"
#include "pxr/base/tf/type.h"

#include "pxr/base/gf/quatd.h"
#include "pxr/base/gf/quatf.h"

#include <cmath>

PXR_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION(TfType) {
    TfType::Define<GfQuath>();
}

GfQuath::GfQuath(GfQuatd const &other)
    : _imaginary(other.GetImaginary())
    , _real(other.GetReal())
{
}
GfQuath::GfQuath(GfQuatf const &other)
    : _imaginary(other.GetImaginary())
    , _real(other.GetReal())
{
}

std::ostream& 
operator<<(std::ostream &out, GfQuath const &q)
{
    GfVec3h i = q.GetImaginary();
    GfHalf real = q.GetReal();
    return out << GfVec4h(real, i[0], i[1], i[2]);
}

GfHalf
GfQuath::Normalize(GfHalf eps)
{
    GfHalf length = GetLength();

    if (length < eps)
        *this = GetIdentity();
    else
        *this /= length;

    return length;
}

// The derivation of this is somewhat obscure so here's how this works.
//
// If we call this quaternion Q, and the point P, the goal is to
// evaluate the expression
//
//     (Q * GfQuath(0, P) * Q.GetInverse()).GetImaginary()
// which evaluates to point, rotated by the quaternion.
// 
// A quaternion has a scalar real part and a vector imaginary part. Call them
// r and i. The multiplication of two quaternions A * B expands to:
//
//     (Ar, Ai)*(Br, Bi) = (Ar*Br-(Ai . Bi), Ar*Bi+Br*Ai+(Ai x Bi))
//                           ^real part^       ^imaginary part^
// 
// where . is vector dot product and x is vector cross product.
//
// Let us define len2 as the quaternion length squared. If the length is 1.0
// (which is true for quaternions representing rotations) len2 will also be 1.0.
//
//     len2 = Q.GetLength()^2
//          = Qr*Qr + (Qi . Qi)
//
// There are a few definitions and identities that will help us out here:
//     Q.GetInverse() == (Qr, -Qi) * s
//     (v1 x v2) = -(v2 x v1)
//     v1 x (v2 x v3) = (v1 . v3)*v2 - (v1 . v2)*v3
//     v1 x (v1 x v2) = (v1 . v2)*v1 - (v1 . v1)*v2
//
// That last identity is a special case of the one above it and it comes up in
// the derivation.
//
// Finally, let R equal the result of transforming P by Q. This gives us the
// quaternion equation:
//     (0, R) = (Qr, Qi) * (0, P) * (Qr, -Qi) / len2
//
// For sake of space, we're going to just assume that the entire right-hand side
// gets divided by len2 and save it out of the equations until the end.
//
//     (0, R) = (-(Qi . P), Qr*P + (Qi x P)) * (Qr, -Qi)
//     (0, R) = ( ..., -(Qi . P)*(-Qi) + Qr*(Qr*P + (Qi x P)) + (Qr*P + (Qi x P)) x (-Qi))
//
// where ... represents the real part which simplifies to 0 and will henceforth be ignored:
//
//     R = -(Qi . P)*(-Qi) + Qr*(Qr*P + (Qi x P)) + (Qr*P + (Qi x P)) x (-Qi)
//       = (Qi . P)*Qi + Qr^2*P + Qr*(Qi x P) + (Qr*P x (-Qi)) + ((Qi x P) x (-Qi))
//       = (Qi . P)*Qi + Qr^2*P + Qr*(Qi x P) + Qr*(Qi x P) + (Qi x (Qi x P))
//       = (Qi . P)*Qi + Qr^2*P + 2*Qr*(Qi x P) + (Qi x (Qi x P))
//       = (Qi . P)*Qi + Qr^2*P + 2*Qr*(Qi x P) + (Qi . P)*Qi - (Qi . Qi)*P
//       = 2*(Qi . P)*Qi + (Qr^2 - (Qi . Qi))*P + 2*Qr*(Qi x P)
//
// now that we've arrived here, lets reintroduce len2 to get:
//
//     R = (2*(Qi . P)*Qi + (Qr^2 - (Qi . Qi))*P + 2*Qr*(Qi x P)) / len2
//
// We can further notice that Qr^2 and (Qi . Qi) both appear in the equation
// above and in the calculation of len2, giving us:
//
//     tmpDot = Qi . Qi
//     tmpSqr = Qr * Qr
//     R = (2*(Qi . P)*Qi + (tmpSqr - tmpDot)*P + 2*Qr*(Qi x P)) / (tmpSqr + tmpDot)
//
// which takes 22 multiplications, 11 additions, and 1 division
//
GfVec3h
GfQuath::Transform(const GfVec3h& point) const
{
    GfHalf tmpDot = GfDot(_imaginary, _imaginary);
    GfHalf tmpSqr = _real * _real;
    return (2 * GfDot(_imaginary, point) * _imaginary +
            (tmpSqr - tmpDot) * point +
            2 * _real * GfCross(_imaginary, point)) / (tmpSqr + tmpDot);
}

GfQuath &
GfQuath::operator *=(const GfQuath &q)
{
    GfHalf r1 = GetReal();
    GfHalf r2 = q.GetReal();
    const GfVec3h &i1 = GetImaginary();
    const GfVec3h &i2 = q.GetImaginary();

    GfHalf r = r1 * r2 - GfDot(i1, i2);

    GfVec3h i(
        r1 * i2[0] + r2 * i1[0] + (i1[1] * i2[2] - i1[2] * i2[1]),
        r1 * i2[1] + r2 * i1[1] + (i1[2] * i2[0] - i1[0] * i2[2]),
        r1 * i2[2] + r2 * i1[2] + (i1[0] * i2[1] - i1[1] * i2[0]));

    SetReal(r);
    SetImaginary(i);

    return *this;
}

GfQuath
GfSlerp(const GfQuath& q0, const GfQuath& q1, double alpha)
{
    return GfSlerp(alpha, q0, q1);
}

GfQuath
GfSlerp(double alpha, const GfQuath& q0, const GfQuath& q1)
{
    double cosTheta = q0.GetImaginary() * q1.GetImaginary() +
        q0.GetReal() * q1.GetReal();
    bool flip1 = false;

    if (cosTheta < 0.0) {
        cosTheta = -cosTheta;
        flip1 = true;
    }

    double scale0, scale1;

    if (1.0 - cosTheta > 0.00001 ) {
        // standard case
        GfHalf theta = std::acos(cosTheta),
               sinTheta = std::sin(theta);

        scale0 = std::sin((1.0 - alpha) * theta) / sinTheta;
        scale1 = std::sin(alpha * theta) / sinTheta;
    } else {        
        // rot0 and rot1 very close - just do linear interp and renormalize.
        scale0 = 1.0 - alpha;
        scale1 = alpha;
    }

    if (flip1)
        scale1 = -scale1;

    return scale0 * q0 + scale1 * q1;
}

PXR_NAMESPACE_CLOSE_SCOPE
