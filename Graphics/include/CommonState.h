#pragma once
#include <d3d11.h>
#include <CommonStates.h>

#include "../export.h"

namespace Global
{
    using namespace DirectX;
    extern GRAPHICS_API CommonStates * cStates;
    extern ID3D11SamplerState * comparisonSampler;
    extern ID3D11SamplerState * mirrorSampler;
    extern ID3D11BlendState * transparencyBlendState;
}