#include "DoFHeader.hlsli"
struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput VS(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.uv = input.uv;
    return output;
}

Texture2D finalTexture : register(t0);

float4 PS(VSOutput input) : SV_Target0
{
    float4 finalColor = finalTexture.Sample(linearSampler, input.uv); 

    finalColor.w = saturate(1000.f * (abs(finalColor) - 0.001f));

    return finalColor;
}