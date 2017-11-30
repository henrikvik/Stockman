#ifndef VERTEX_HLSLI
#define VERTEX_HLSLI

struct Vertex
{
    float3 position        : POSITION     ;
    float3 normal          : NORMAL       ;
    float3 binormal        : BINORMAL     ;
    float3 tangent         : TANGENT      ;
    float2 uv              : UV           ;
    uint4   jointIds       : JOINT_IDS    ;
    float4  jointWeights   : JOINT_WEIGHTS;
};

#endif