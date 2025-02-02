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
#include "pxr/imaging/hd/renderPassState.h"

#include "pxr/imaging/hd/camera.h"
#include "pxr/imaging/hd/changeTracker.h"
#include "pxr/imaging/hd/debugCodes.h"
#include "pxr/imaging/hd/tokens.h"

#include "pxr/base/gf/camera.h"
#include "pxr/base/gf/frustum.h"
#include "pxr/base/tf/stringUtils.h"

PXR_NAMESPACE_OPEN_SCOPE

HdRenderPassState::HdRenderPassState()
    : _camera(nullptr)
    , _viewport(0, 0, 1, 1)
    , _overrideWindowPolicy{false, CameraUtilFit}
    , _cullMatrix(1)
    , _worldToViewMatrix(1)
    , _projectionMatrix(1)

    , _overrideColor(0.0f, 0.0f, 0.0f, 0.0f)
    , _wireframeColor(0.0f, 0.0f, 0.0f, 0.0f)
    , _pointColor(0.0f, 0.0f, 0.0f, 1.0f)
    , _pointSize(3.0)
    , _lightingEnabled(true)
    , _clippingEnabled(true)

    , _maskColor(1.0f, 0.0f, 0.0f, 1.0f)
    , _indicatorColor(0.0f, 1.0f, 0.0f, 1.0f)
    , _pointSelectedSize(3.0)

    , _alphaThreshold(0.5f)
    , _tessLevel(32.0)
    , _drawRange(0.9, -1.0)

    , _depthBiasUseDefault(true)
    , _depthBiasEnabled(false)
    , _depthBiasConstantFactor(0.0f)
    , _depthBiasSlopeFactor(1.0f)
    , _depthFunc(HdCmpFuncLEqual)
    , _depthMaskEnabled(true)
    , _depthTestEnabled(true)
    , _cullStyle(HdCullStyleNothing)
    , _stencilFunc(HdCmpFuncAlways)
    , _stencilRef(0)
    , _stencilMask(~0)
    , _stencilFailOp(HdStencilOpKeep)
    , _stencilZFailOp(HdStencilOpKeep)
    , _stencilZPassOp(HdStencilOpKeep)
    , _stencilEnabled(false)
    , _lineWidth(1.0f)
    , _blendColorOp(HdBlendOpAdd)
    , _blendColorSrcFactor(HdBlendFactorOne)
    , _blendColorDstFactor(HdBlendFactorZero)
    , _blendAlphaOp(HdBlendOpAdd)
    , _blendAlphaSrcFactor(HdBlendFactorOne)
    , _blendAlphaDstFactor(HdBlendFactorZero)
    , _blendConstantColor(0.0f, 0.0f, 0.0f, 0.0f)
    , _blendEnabled(false)
    , _alphaToCoverageEnabled(false)
    , _colorMaskUseDefault(true)
    , _useMultiSampleAov(true)

{
}

HdRenderPassState::~HdRenderPassState() = default;

/* virtual */
void
HdRenderPassState::Prepare(HdResourceRegistrySharedPtr const &resourceRegistry)
{
    if(!TfDebug::IsEnabled(HD_FREEZE_CULL_FRUSTUM)) {
        _cullMatrix = GetWorldToViewMatrix() * GetProjectionMatrix();
    }
}

void
HdRenderPassState::SetCameraAndViewport(HdCamera const *camera,
                                        GfVec4d const &viewport)
{
    if (!camera) {
        TF_CODING_ERROR("Received null camera\n");
    }
    _camera = camera;
    _viewport = GfVec4f((float)viewport[0], (float)viewport[1],
                        (float)viewport[2], (float)viewport[3]);

    // Invalidate framing so that it isn't used by GetProjectionMatrix().
    _framing = CameraUtilFraming();
}

void
HdRenderPassState::SetCameraAndFraming(
    HdCamera const *camera,
    const CameraUtilFraming &framing,
    const std::pair<bool, CameraUtilConformWindowPolicy> &overrideWindowPolicy)
{
    if (!camera) {
        TF_CODING_ERROR("Received null camera\n");
    }
    _camera = camera;
    _framing = framing;
    _overrideWindowPolicy = overrideWindowPolicy;
}

GfMatrix4d
HdRenderPassState::GetWorldToViewMatrix() const
{
    if (!_camera) {
        return _worldToViewMatrix;
    }

    return _camera->GetViewMatrix();
}

CameraUtilConformWindowPolicy
HdRenderPassState::GetWindowPolicy() const
{
    if (_overrideWindowPolicy.first) {
        return _overrideWindowPolicy.second;
    }
    if (_camera) {
        return _camera->GetWindowPolicy();
    }

    return CameraUtilFit;
}

GfMatrix4d
HdRenderPassState::GetProjectionMatrix() const
{
    if (!_camera) {
        return _projectionMatrix;
    }

    if (_framing.IsValid()) {
        return
            _framing.ApplyToProjectionMatrix(
                _camera->GetProjectionMatrix(),
                GetWindowPolicy());
    }

    CameraUtilConformWindowPolicy const policy = _camera->GetWindowPolicy();
    const double aspect =
        (_viewport[3] != 0.0 ? _viewport[2] / _viewport[3] : 1.0);

    // Adjust the camera frustum based on the window policy.
    return CameraUtilConformedWindow(
        _camera->GetProjectionMatrix(), policy, aspect);
}

HdRenderPassState::ClipPlanesVector const &
HdRenderPassState::GetClipPlanes() const
{
    if (!_clippingEnabled) {
        const static HdRenderPassState::ClipPlanesVector empty;
        return empty;
    }

    if (!_camera) {
        return _clipPlanes;
    }
    return _camera->GetClipPlanes();
}

void
HdRenderPassState::SetOverrideColor(GfVec4f const &color)
{
    _overrideColor = color;
}

void
HdRenderPassState::SetWireframeColor(GfVec4f const &color)
{
    _wireframeColor = color;
}

void
HdRenderPassState::SetMaskColor(GfVec4f const &color)
{
    _maskColor = color;
}

void
HdRenderPassState::SetIndicatorColor(GfVec4f const &color)
{
    _indicatorColor = color;
}

void
HdRenderPassState::SetPointColor(GfVec4f const &color)
{
    _pointColor = color;
}

void
HdRenderPassState::SetPointSize(float size)
{
    _pointSize = size;
}

void
HdRenderPassState::SetPointSelectedSize(float size)
{
    _pointSelectedSize = size;
}

void
HdRenderPassState::SetCullStyle(HdCullStyle cullStyle)
{
    _cullStyle = cullStyle;
}

void
HdRenderPassState::SetAlphaThreshold(float alphaThreshold)
{
    _alphaThreshold = alphaThreshold;
}

void
HdRenderPassState::SetTessLevel(float tessLevel)
{
    _tessLevel = tessLevel;
}

void
HdRenderPassState::SetDrawingRange(GfVec2f const &drawRange)
{
    _drawRange = drawRange;
}

void
HdRenderPassState::SetLightingEnabled(bool enabled)
{
    _lightingEnabled = enabled;
}

void
HdRenderPassState::SetClippingEnabled(bool enabled)
{
    _clippingEnabled = enabled;
}

void
HdRenderPassState::SetAovBindings(
        HdRenderPassAovBindingVector const& aovBindings)
{
    _aovBindings= aovBindings;
}

HdRenderPassAovBindingVector const&
HdRenderPassState::GetAovBindings() const
{
    return _aovBindings;
}

void
HdRenderPassState::SetAovInputBindings(
        HdRenderPassAovBindingVector const& aovBindings)
{
    _aovInputBindings= aovBindings;
}

HdRenderPassAovBindingVector const&
HdRenderPassState::GetAovInputBindings() const
{
    return _aovInputBindings;
}

void
HdRenderPassState::SetUseAovMultiSample(bool state)
{
    _useMultiSampleAov = state;
}

bool
HdRenderPassState::GetUseAovMultiSample() const
{
    return _useMultiSampleAov;
}

void
HdRenderPassState::SetDepthBiasUseDefault(bool useDefault)
{
    _depthBiasUseDefault = useDefault;
}

void
HdRenderPassState::SetDepthBiasEnabled(bool enable)
{
    _depthBiasEnabled = enable;
}

void
HdRenderPassState::SetDepthBias(float constantFactor, float slopeFactor)
{
    _depthBiasConstantFactor = constantFactor;
    _depthBiasSlopeFactor = slopeFactor;
}

void
HdRenderPassState::SetDepthFunc(HdCompareFunction depthFunc)
{
    _depthFunc = depthFunc;
}

void
HdRenderPassState::SetEnableDepthMask(bool state)
{
    _depthMaskEnabled = state;
}

bool
HdRenderPassState::GetEnableDepthMask()
{
    return _depthMaskEnabled;
}

void
HdRenderPassState::SetEnableDepthTest(bool enabled)
{
    _depthTestEnabled = enabled;
}

bool
HdRenderPassState::GetEnableDepthTest() const
{
    return _depthTestEnabled;
}

void
HdRenderPassState::SetStencil(HdCompareFunction func,
        int ref, int mask,
        HdStencilOp fail, HdStencilOp zfail, HdStencilOp zpass)
{
    _stencilFunc = func;
    _stencilRef = ref;
    _stencilMask = mask;
    _stencilFailOp = fail;
    _stencilZFailOp = zfail;
    _stencilZPassOp = zpass;
}

void
HdRenderPassState::SetStencilEnabled(bool enabled)
{
    _stencilEnabled = enabled;
}

bool
HdRenderPassState::GetStencilEnabled() const
{
    return _stencilEnabled;
}

void
HdRenderPassState::SetLineWidth(float width)
{
    _lineWidth = width;
}

void
HdRenderPassState::SetBlend(HdBlendOp colorOp,
                            HdBlendFactor colorSrcFactor,
                            HdBlendFactor colorDstFactor,
                            HdBlendOp alphaOp,
                            HdBlendFactor alphaSrcFactor,
                            HdBlendFactor alphaDstFactor)
{
    _blendColorOp = colorOp;
    _blendColorSrcFactor = colorSrcFactor;
    _blendColorDstFactor = colorDstFactor;
    _blendAlphaOp = alphaOp;
    _blendAlphaSrcFactor = alphaSrcFactor;
    _blendAlphaDstFactor = alphaDstFactor;
}

void
HdRenderPassState::SetBlendConstantColor(GfVec4f const & color)
{
    _blendConstantColor = color;
}

void
HdRenderPassState::SetBlendEnabled(bool enabled)
{
    _blendEnabled = enabled;
}

void
HdRenderPassState::SetAlphaToCoverageEnabled(bool enabled)
{
    _alphaToCoverageEnabled = enabled;
}

void
HdRenderPassState::SetColorMaskUseDefault(bool useDefault)
{
    _colorMaskUseDefault = useDefault;
}

void
HdRenderPassState::SetColorMasks(
    std::vector<HdRenderPassState::ColorMask> const& masks)
{
    _colorMasks = masks;
}

PXR_NAMESPACE_CLOSE_SCOPE

