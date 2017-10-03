cbuffer Camera : register(b0)
{
	float4x4 PV;
};

struct FogData
{
	float3 pos;
	float4 color;
};

StructuredBuffer<FogData> fogData : register(t0);

struct VSOut 
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 aV : AV;
	float c1 : C1;
	float c2 : C2;
	float F_DOT_V : F_DOT_V;
};

VSOut VS(uint id : SV_VertexID)
{
	VSOut vsOut;
	
	//F is the four dimensional vector that represents the bounding plane 
	float4 F;

	//a is a random posetive constant
	float a = 1;
	
	float k;
	if ()
	{

	}

	vsOut.pos = mul(PV, float4(fogData[id].pos, 1));
	vsOut.color = fogData[id].color;

	return vsOut;
}

float4 PS(VSOut psIn) : SV_Target
{
	float3 fogColor = (0.5 ,0.5, 0.5);
	float g;

	float f;

	psIn.color.rgb = psIn.color.rgb * f + fogColor * (1.0 - f);
	//psIn.color.a = ??

	return float4(psIn.color, 1);
}