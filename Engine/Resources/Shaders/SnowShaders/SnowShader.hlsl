#include "../LightCalcInclude.hlsli"

#define SNOW_RADIUS 50.f

struct Vertex
{
    float3 pos : POSITION;
    float rot  : ROTATIONVALUE;
    float distance : DISTANCE;
};

StructuredBuffer<Vertex> vertexBuffer : register(t4);

Vertex VS(uint vertexId : SV_VertexId)
{
    return vertexBuffer[vertexId];
}

struct GS_OUT
{
    float4 pos : SV_Position;
    float distance : DISTANCE;
};

[maxvertexcount(3)]
void GS(point Vertex input[1], inout TriangleStream<GS_OUT> output)
{
    GS_OUT hablaPos = (GS_OUT)0;
    hablaPos.distance = input[0].distance;

    float rot = input[0].rot;
    float4x4 rotation = 
    {
        cos(rot), -sin(rot),  -sin(rot),      0,

        sin(rot),  cos(rot),   cos(rot),      0,

        sin(rot),  sin(rot),   cos(rot),      0,

            0,         0,          0,         1
    };

    float4 positions[3] = 
    { 
          0, 0, 0, 0,
          1, 0, 0, 0,
        0.5, 1, 0, 0
    };

    for (int i = 0; i < 3; i++)
    {
        positions[i] = mul(rotation, positions[i]);
    }

    float4 temp = float4(input[0].pos, 1);
    hablaPos.pos = temp.xyzw + positions[0];
    hablaPos.pos = mul(ViewProjection, hablaPos.pos);
    output.Append(hablaPos);

    hablaPos.pos = temp + positions[1];
    hablaPos.pos = mul(ViewProjection, hablaPos.pos);
    output.Append(hablaPos);

    hablaPos.pos = temp + positions[2];
    hablaPos.pos = mul(ViewProjection, hablaPos.pos);
    output.Append(hablaPos);
}


float4 PS(GS_OUT input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1 - input.distance / SNOW_RADIUS);
}