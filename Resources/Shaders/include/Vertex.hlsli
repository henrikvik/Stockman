#ifndef VERTEX_HLSLI
#define VERTEX_HLSLI

struct Vertex
{
    float3 position;
    float3 normal;
    float3 binormal;
    float3 tangent;
    float2 uv;
    uint   jointIds[4];
    float  jointWeights[4];
};

#endif