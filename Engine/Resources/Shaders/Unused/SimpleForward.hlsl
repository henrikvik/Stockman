#include "Vertex.hlsl"

cbuffer Camera : register(b0)
{
    float4x4 PV;
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

struct FragmentInfo
{
    float4 ndcPos : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
};


FragmentInfo VS(InstancedVertex vertex)
{
    FragmentInfo fragment = (FragmentInfo)0;

    fragment.worldPos = mul(vertex.world, float4(vertex.position, 1));
    fragment.normal = normalize(mul(vertex.world, float4(vertex.normal, 0)));

    fragment.ndcPos = mul(PV, float4(fragment.worldPos, 1));

    return fragment;
}


float4 PS(FragmentInfo fragment) : SV_Target
{
    float3 output = float3(1, 0, 0);
    float3 lightPos = float3(0, 10, -10);

    float diffuseFactor = saturate(dot(fragment.normal, normalize(lightPos - fragment.worldPos)));
    float ambient = 0.1;

    return float4(output * (diffuseFactor + ambient), 1);
}