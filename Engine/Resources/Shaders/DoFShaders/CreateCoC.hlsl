#include "DoFHeader.hlsli"
struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput VS( VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.uv = input.uv;
    return output;
}

Texture2D colorBuffer : register(t0);
Texture2D depthBuffer : register(t1);

float4 PS(VSOutput input) : SV_Target0
{
    float4 color;
    color.xyz = colorBuffer.Sample(linearSampler, input.uv).xyz;
    color.w = depthBuffer.Sample(pointSampler, input.uv).x;

    //color.w = 0.5f -((CoCScale * color.w) + CoCBias);
    color.w = ((CoCScale * color.w) + CoCBias);
    return color;
}