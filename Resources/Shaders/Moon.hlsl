#include "include/Camera.hlsli"
cbuffer cb0 : register(b0) { Camera camera; };
cbuffer SkyParams : register(b1) { 
    float4 Base;
    float4 Top;
    float4 AngleSize;
    float4x4 View;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VSOut VS(uint id: SV_VertexID)
{
    VSOut vsout;

    vsout.uv = float2((id << 1) & 2, id & 2);
    vsout.pos = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);
    //vsout.pos *= 50;


    float4 p = mul(View, float4(vsout.pos.xyz * AngleSize.w, 1.f));
    //p.w = 1.f;
    //vsout.pos = p;
    vsout.pos = mul(camera.viewProjection, p.xyzw);
    //vsout.pos.z = 1.f;
    //vsout.pos.w = 1.f;

    return vsout;
}

Texture2D MoonTexture : register(t0);
SamplerState LinearClamp : register(s0);

float4 PS(VSOut input) : SV_Target0
{
    float4 col = MoonTexture.Sample(LinearClamp, input.uv);
    
    clip(col.a - 0.5);

    return col;
}