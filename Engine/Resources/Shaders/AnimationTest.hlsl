cbuffer Camera : register(b0)
{
    float4x4 VP;
};

StructuredBuffer<float4x4> jointTransforms : register(t0);

struct VertexData
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    uint4 jointIds : JOINTIDS;
    float4 jointWeights : WEIGHTS;
};

struct FragmentData
{
    float4 ndcPos : SV_Position;
    float3 normal : Normal;
};

FragmentData VS(VertexData vertex, uint vertexId : SV_VertexId)
{
    FragmentData fragment;

    fragment.ndcPos = mul(VP, mul(jointTransforms[vertex.jointIds[0]], float4(vertex.pos, 1)));
    fragment.normal = vertex.normal; //mul(VP, float4(vertex.normal, 0));

    return fragment;
}

float4 PS(FragmentData fragment) : SV_Target
{
    return float4(abs(fragment.normal), 1);
}