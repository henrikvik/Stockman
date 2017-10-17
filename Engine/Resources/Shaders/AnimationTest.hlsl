cbuffer Camera : register(b0)
{
    float4x4 VP;
    float4x4 _a;
    float4x4 _b;
    float4 cameraPosition;
};

struct Vertex
{
    float3 position;
    float3 normal;
    float3 binormal;
    float3 tangent;
    float2 uv;
    uint jointIds[4];
    float jointWeights[4];
};

StructuredBuffer<Vertex>   vertexData      : register(t0);
StructuredBuffer<float4x4> jointTransforms : register(t1);

struct FragmentData
{
    float4 ndcPos : SV_Position;
    float3 normal : Normal;
    float4 color : Color;
};

float4x4 calculateAnimationMatrix(Vertex vertex)
{
    float4x4 animMatrix;
    float4x4 identity = float4x4(
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    );

    for (int i = 0; i < 4; i++)
    {
        animMatrix += mul(jointTransforms[vertex.jointIds[i]], vertex.jointWeights[i]);
    }

    return animMatrix;

}

FragmentData VS(uint vertexId : SV_VertexId)
{
    Vertex vertex = vertexData[vertexId];

    FragmentData fragment;
    
    float4 position = float4(0.0, 0.0, 0.0, 0.0);
    float4 normal = float4(0.0, 0.0, 0.0, 0.0);

    for (int i = 0; i < 4; i++)
    {
        float4 jointPosition = mul(jointTransforms[vertex.jointIds[i]], float4(vertex.position, 1));
        position += jointPosition * vertex.jointWeights[i];

        float4 jointNormal = mul(jointTransforms[vertex.jointIds[i]], float4(vertex.normal, 0));
        normal += jointNormal * vertex.jointWeights[i];
    }

    fragment.ndcPos = mul(VP, position);
    fragment.normal = mul(VP, float4(vertex.normal, 0));
    fragment.color = float4(abs(fragment.normal), 1);

    //float4x4 animMatrix = calculateAnimationMatrix(vertex);
    //fragment.ndcPos = mul(VP, mul(animMatrix, float4(vertex.position, 1)));
    //fragment.normal = mul(VP, mul(animMatrix, float4(vertex.normal, 0)));

    return fragment;
}

float4 PS(FragmentData fragment) : SV_Target
{
    return fragment.color;
}