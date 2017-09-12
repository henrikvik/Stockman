#include "Vertex.hlsl"

cbuffer Camera : register(b0)
{
    float4x4 VP;
};


struct VertexInstance : Vertex
{
    float4x4 translation : TRANSLATION;
};



float4 VS(Vertex input) : SV_POSITION
{
    return mul(input.translation * VP, float4(input.position, 1));
}

float4 PS(float4 ndcPos) : SV_Target0
{
    return float4(1, 0, 0, 1);
}