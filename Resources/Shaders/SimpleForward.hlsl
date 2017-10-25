#define VERTEX_T_SLOT t4
#include "Vertex.hlsli"

#define INSTANCE_T_SLOT t5
#include "Instance.hlsli"


Texture2D diffuse : register(t10);
SamplerState pointSampler : register(s0);

cbuffer Camera : register(b0)
{
    float4x4 PV;
};

struct FragmentInfo
{
    float4 ndcPos : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

FragmentInfo VS(uint vertexId : SV_VertexId, uint instanceId : SV_InstanceId)
{
    Vertex vertex = getVertex(vertexId);
    Instance instance = getInstance(instanceId);

    FragmentInfo fragment;

    fragment.worldPos = mul(instance.world, float4(vertex.position, 1));
    fragment.normal = normalize(mul(instance.world, float4(vertex.normal, 0)));

    fragment.ndcPos = mul(PV, float4(fragment.worldPos, 1));
    fragment.uv = vertex.uv;

    return fragment;
}


float4 PS(FragmentInfo fragment) : SV_Target
{
    return diffuse.Sample(pointSampler, fragment.uv);
}