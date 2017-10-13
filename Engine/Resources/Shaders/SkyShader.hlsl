#include "ShaderConstants.hlsli"

struct VS_IN
{
    float3 pos : POSITION;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 tex : TEXCOORDS;
};

cbuffer VP : register(b0)
{
    float4x4 VP;
};

cbuffer transform : register(b1)
{
    float4x4 transform;
};

cbuffer colorChange : register(b2)
{
    float4 nope;
    float3 color;
};

cbuffer BulletTimeTimer : register(b3)
{
    float bulletTimer;
};


PS_IN VS(VS_IN input)
{
    PS_IN output = (PS_IN)0;

    output.pos = float4(input.pos, 1);

    output.pos = mul(transform, output.pos);
    output.pos = mul(VP, output.pos).xyww;
    output.tex = input.pos;

    return output;
}

TextureCube cube : register(t0);
SamplerState sState;

float4 PS(PS_IN input) : SV_Target0
{
    float3 finalColor = cube.Sample(sState, input.tex).xyz * color;
    finalColor = adjustSaturation(finalColor, bulletTimer);
    finalColor = adjustContrast(finalColor, 2 - bulletTimer, 0.3);
    return float4(finalColor, 1);
}