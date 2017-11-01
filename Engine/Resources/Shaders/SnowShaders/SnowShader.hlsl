#include "../LightCalcInclude.hlsli"

struct Vertex
{
    float4 pos : POSITION;
};

StructuredBuffer<Vertex> vertexBuffer : register(t4);

float3 VS(uint vertexId : SV_VertexId) : POS
{
    return vertexBuffer[vertexId].pos;
}

struct GS_OUT
{
    float4 pos : SV_Position;
};

[maxvertexcount(3)]
void GS(point float3 input[1] : POS, inout TriangleStream<GS_OUT> output)
{
    GS_OUT hablaPos = (GS_OUT)0;

    float4 temp = mul(ViewProjection, float4(input[0], 1));
    hablaPos.pos = temp.xyzw / temp.w;

    output.Append(hablaPos);
    hablaPos.pos = (temp + float4(1, 0.0, 0.0, 0)) / temp.w;
    output.Append(hablaPos);
    hablaPos.pos = (temp + float4(0.0, 1, 0.0, 0)) / temp.w;
    output.Append(hablaPos);
}


float4 PS(GS_OUT input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}