#ifdef VERTEX_T_SLOT
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
StructuredBuffer<Vertex> VERTEX_DATA : register(VERTEX_T_SLOT);

Vertex getVertex(uint vertexId)
{
    return VERTEX_DATA[vertexId];
}
#else
#error "VERTEX_T_SLOT not defined, nowhere to bind VERTEX_DATA"
#endif