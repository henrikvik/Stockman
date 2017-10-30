#include "ShaderConstants.hlsli"

#define VERTEX_T_SLOT t0
#include "Vertex.hlsli"

struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 lPos : WORLD_POSITION;
    float2 uv : UV;
};

cbuffer VP : register(b0)
{
    float4x4 VP;
};

cbuffer transform : register(b1)
{
    float4x4 transform;
};

cbuffer LightBuffer : register(b2)
{
    float4 dirLightPos;
    float fade;
}

cbuffer BulletTimeTimer : register(b3)
{
    float bulletTimer;
};


PS_IN VS(uint vertexId : SV_VertexId)
{
    Vertex vertex = getVertex(vertexId);
    PS_IN output = (PS_IN) 0;

    output.pos = float4(vertex.position, 1);

    output.pos = mul(transform, output.pos);
    output.pos = mul(VP, output.pos).xyww;
    output.uv = vertex.uv;

    output.lPos = vertex.position;

    return output;
}

float4 PS(PS_IN input) : SV_Target0
{
    float3 dayColor1 = float3(0, 0.5, 0.8);
    float3 dayColor2 = float3(0.8, 0.8, 0.8);

    float3 dawnColor1 = float3(0.4, 0.3, 0.6);
    float3 dawnColor2 = float3(1, 0.5, 0);

    float3 nightColor1 = float3(0, 0.0, 0.05);
    float3 nightColor2 = float3(0.0, 0.1, 0.3);

    float3 dayGradient = lerp(dayColor2, dayColor1, input.lPos.y);
    float3 dawnGradient = lerp(dawnColor2, dawnColor1, input.lPos.y);
    float3 nightGradient = lerp(nightColor2, nightColor1, input.lPos.y);
    
    float3 dayDawnGradient = lerp(dayGradient, dawnGradient, 1 - fade);
    float3 dawnNightGradient = lerp(dawnGradient, nightGradient, 1 - fade);
    
    

    float3 gradient = lerp(dayDawnGradient, dawnNightGradient, 1 - fade);

    nightGradient = adjustSaturation(nightGradient, bulletTimer);
    nightGradient = adjustContrast(nightGradient, 2 - bulletTimer, 0.3);

    return float4(nightGradient, 1);
}