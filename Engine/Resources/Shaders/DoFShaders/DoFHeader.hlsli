#pragma once


struct VSInput
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD;
};

SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);

#define CocBias 1.0f
#define CoCScale 1.0f