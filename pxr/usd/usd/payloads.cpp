//
// Copyright 2019 Pixar
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
#include "pxr/usd/usd/common.h"
#include "pxr/usd/usd/payloads.h"
#include "pxr/usd/usd/listEditImpl.h"

PXR_NAMESPACE_OPEN_SCOPE

// ------------------------------------------------------------------------- //
// UsdPayloads
// ------------------------------------------------------------------------- //

using _PayloadsListEditImpl = 
    Usd_ListEditImpl<UsdPayloads, SdfPayloadsProxy>;

// The implementation doesn't define this function as it needs to be specialized
// so we implement it here.
template <>
SdfPayloadsProxy 
_PayloadsListEditImpl::_GetListEditorForSpec(const SdfPrimSpecHandle &spec)
{
    return spec->GetPayloadList();
}

bool
UsdPayloads::AddPayload(const SdfPayload& refIn, UsdListPosition position)
{
    return _PayloadsListEditImpl::Add(*this, refIn, position);
}

bool
UsdPayloads::AddPayload(const std::string &assetPath,
                        const SdfPath &primPath,
                        const SdfLayerOffset &layerOffset,
                        UsdListPosition position)
{
    SdfPayload payload(assetPath, primPath, layerOffset);
    return AddPayload(payload, position);
}

bool
UsdPayloads::AddPayload(const std::string &assetPath,
                        const SdfLayerOffset &layerOffset,
                        UsdListPosition position)
{
    SdfPayload payload(assetPath, SdfPath(), layerOffset);
    return AddPayload(payload, position);
}

bool 
UsdPayloads::AddInternalPayload(const SdfPath &primPath,
                                const SdfLayerOffset &layerOffset,
                                UsdListPosition position)
{
    SdfPayload payload(std::string(), primPath, layerOffset);
    return AddPayload(payload, position);
}

bool
UsdPayloads::RemovePayload(const SdfPayload& refIn)
{
    return _PayloadsListEditImpl::Remove(*this, refIn);
}

bool
UsdPayloads::ClearPayloads()
{
    return _PayloadsListEditImpl::Clear(*this);
}

bool 
UsdPayloads::SetPayloads(const SdfPayloadVector& itemsIn)
{
    return _PayloadsListEditImpl::Set(*this, itemsIn);
}

PXR_NAMESPACE_CLOSE_SCOPE

