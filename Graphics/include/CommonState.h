#pragma once
#include <d3d11.h>
#include <CommonStates.h>



namespace Global
{
    using namespace DirectX;
    extern CommonStates * cStates;
    extern ID3D11SamplerState * comparisonSampler;
    extern ID3D11SamplerState * mirrorSampler;
}