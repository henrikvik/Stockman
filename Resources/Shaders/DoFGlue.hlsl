#include "DoFHeader.hlsli"
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

Texture2D finalTexture : register(t0);

float4 PS(VSOutput input) : SV_Target0
{
    half4 finalColor = finalTexture.Sample(linearSampler, input.uv); 

    finalColor.w = saturate(1000.f * (abs(finalColor) - 0.001f));

    return finalColor;
}