#ifndef FRAGMENT_HLSLI
#define FRAGMENT_HLSLI

struct Fragment
{
    float4 ndcPosition : SV_Position;
    float4 position    : Position;

    float3 normal     : Normal;
    float3 binormal   : Binormal;
    float3 tangent    : Tangent;
    float4 viewNormal : ViewNormal;
    
    float2 uv     : UV;
    float2 gridUV : GridUV;
};

#endif