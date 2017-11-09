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