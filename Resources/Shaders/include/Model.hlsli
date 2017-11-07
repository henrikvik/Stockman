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

StructuredBuffer<Vertex> vertexBuffer : register(t11);
Texture2D diffuseMap  : register(t12);
Texture2D normalMap   : register(t13);
Texture2D specularMap : register(t14);
Texture2D glowMap     : register(t15);

Vertex getVertex(uint vertexId)
{
    return vertexBuffer[vertexId];
}