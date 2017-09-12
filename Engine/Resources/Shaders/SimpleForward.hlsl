#include "Vertex.hlsl"

cbuffer Camera : register(b0)
{
    float4x4 VP;
};


struct InstancedVertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float2 biTangent : BITANGENT;
    float2 tangent : TANGENT;

    // INSTANCE DATA
    float4x4 world : WORLD;
};


float4 VS(InstancedVertex input) : SV_POSITION
{
    return mul(input.world * VP, float4(input.position, 1));
}

float4 PS(float4 ndcPos : SV_POSITION) : SV_Target
{
    float3 output = float3(1, 0, 0);
    return float4(output, 1);
}