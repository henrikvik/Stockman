#include "DoFHeader.hlsli"
struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer CoCbuffer : register(b0)
{
    float CoCScale;
    float CoCBias;
}

Texture2D colorBuffer : register(t0);
Texture2D depthBuffer : register(t1);

float getLinearDepth(float near, float far, float2 uv)
{
    return 2.0 * near / (far + near - depthBuffer.SampleLevel(pointSampler, uv, 0).r * (far - near));
}


VSOutput VS(uint id: SV_VertexID)
{
    VSOutput vsout;
    
	vsout.uv = float2((id << 1) & 2, id & 2);
	vsout.pos = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);


    return vsout;
}



float4 PS(VSOutput input) : SV_Target0
{
    float4 color;
    color.xyz = colorBuffer.Sample(linearSampler, input.uv).xyz;
    color.w = depthBuffer.Sample(pointSampler, input.uv);
    //color.w = getLinearDepth(nearP, farP, input.uv);

    color.w = ((CoCScale * color.w) + CoCBias);
    //color.w *= 100.f;
    return color;
}

