#include "include/Camera.hlsli"

struct Vertex
{
    float4 position : Position;
    float4 color : Color;
};

struct Fragment
{
    float4 ndcPosition : SV_Position;
    float4 color : Color;
};

cbuffer cb0 : register(b0) { Camera camera; }
cbuffer cb1 : register(b1) { uint offset;   }

StructuredBuffer<Vertex> vertexBuffer : register(t0);

Fragment VS(uint vertexId : SV_VertexId)
{
    Vertex vertex = vertexBuffer[offset + vertexId]; 
    Fragment fragment;

    fragment.ndcPosition = mul(camera.viewProjection, vertex.position);
    fragment.color = vertex.color;

    return fragment;
}

float4 PS(Fragment fragment) : SV_Target0
{
    return fragment.color;
}