cbuffer Camera : register(b0)
{
	float4x4 PV;
};

struct FogData
{
	float3 pos;
	float3 color;
}

StructuredBuffer<FogData> fogData : register(t0);

struct VSOut 
{
	float4 pos : SV_POSITION;
	float3 color : color;
};

VSOut VS(uint id : SV_VertexID)
{
	VSOut vsOut;

	vsOut.pos = mul(PV,fogData[id].pos);
	vsOut.color = fogData[id].color;

	return vsOut
}

float4 PS(VSOut psIn) : SV_Target
{


	return float4(psIn.color, 1);
}}