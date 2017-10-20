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

StructuredBuffer<Vertex> vertexData : register(t0);
StructuredBuffer<float4x4> jointTransforms : register(t1);
StructuredBuffer<float4x4> instanceTransforms : register(t2);

float4x4 calculateAnimationMatrix(Vertex vertex)
{
    float4x4 animMatrix;

    for (int i = 0; i < 4; i++)
    {
        animMatrix += mul(jointTransforms[vertex.jointIds[i]], vertex.jointWeights[i]);
    }

    return animMatrix;
}

Fragment VS(uint vertexId : SV_VertexId, uint instanceId : SV_InstanceId)
{
    Vertex vertex = vertexData[vertexId];
    float4x4 world = instanceTransforms[instanceId];
    Fragment fragment;

    float4x4 animMatrix = calculateAnimationMatrix(vertex);
    fragment.ndcPos = mul(VP, mul(world, float4(vertex.position, 1)));
    fragment.normal = mul(VP, mul(world, float4(vertex.normal, 0)));
    fragment.color = float4(1,1,1,1);

    return fragment;
}

Pixel PS(Fragment fragment)
{
    Pixel pixel;

    pixel.color = fragment.color;

    return pixel;
}