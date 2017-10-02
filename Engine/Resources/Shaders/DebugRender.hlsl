cbuffer Camera : register(b0)
{
    float4x4 PV;
};

cbuffer Color : register(b1)
{
    float4 debugColor;
};

StructuredBuffer<float3> points : register(t0);


float4 VS(uint id : SV_VertexID) : SV_Position
{
    return mul(PV, float4(points[id], 1));
}

float4 PS(float4 ndc : SV_Position) : SV_Target
{
    return debugColor;
}