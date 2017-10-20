#define VERTEX_T_SLOT t0
#include "Vertex.hlsli"

#define ANIMATION_T_SLOT t1
#include "Animation.hlsli"


struct Fragment
{
    float4 ndcPos : SV_Position;
    float3 normal : Normal;
    float4 color  : Color;
};

struct Pixel
{
    float4 color : SV_Target0;
};

cbuffer Camera : register(b0)
{
    float4x4 VP;
    float4x4 _a;
    float4x4 _b;
    float4 cameraPosition;
};


Fragment VS(uint vertexId : SV_VertexId)
{
    Vertex vertex = getVertex(vertexId);
    Fragment fragment;
    
    float4x4 animMatrix = calculateAnimationMatrix(vertex);
    fragment.ndcPos = mul(VP, mul(animMatrix, float4(vertex.position, 1)));
    fragment.normal = mul(VP, mul(animMatrix, float4(vertex.normal, 0)));
    fragment.color  = float4(abs(vertex.normal), 1);

    return fragment;
}

Pixel PS(Fragment fragment)
{
    Pixel pixel;

    pixel.color = fragment.color;

    return pixel;
}