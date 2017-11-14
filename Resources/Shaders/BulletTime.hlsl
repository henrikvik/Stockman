#include "include/LightCalc.hlsli"

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput VS(uint id: SV_VertexID)
{
    VSOutput vsout;
    
	vsout.uv = float2((id << 1) & 2, id & 2);
	vsout.pos = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);


    return vsout;
}
cbuffer timer : register(b0)
{
    float progress;
}

Texture2D tex : register(t0);
SamplerState sState : register(s0);

float4 PS(VSOutput input) : SV_Target0
{
    float3 color = tex.Sample(sState, input.uv);

    color = adjustContrast(color, 2 - progress, 0.3f);
    color = adjustSaturation(color, progress);

    return float4(color, 1);
}