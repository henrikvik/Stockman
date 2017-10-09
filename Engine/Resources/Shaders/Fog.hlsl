cbuffer Camera : register(b0)
{
	float4x4 PV;
	float4x4 InvV;
	float4x4 V;
	float4 camPos;
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
	float4 color : COLOR;
	float3 aV : AV;
	float c1 : C1;
	float c2 : C2;
	float F_DOT_V : F_DOT_V;
};

VSOut VS(uint id : SV_VertexID)
{
	VSOut vsOut;
	
	//F is the four dimensional vector that represents the bounding plane, XYZ is the normal of the plane. 
	float4 F = float4(0,-1,0,1);
	F = normalize(F);
	//ViewVec
	float4 P = float4(fogData[id].pos, 1);
	float4 V = camPos - P;
	//a is a random posetive constant, NOTE let it be over 20 or the color of
	//the fragment is going to "shine through" when under the fog halfspace
	float a = 20;
	//K is either 1 or 0
	float k = 0;

	if (dot(F, camPos) <= 0)
	{
		k = 1;
	}

	vsOut.aV = (a / 2) * V.xyzw;
	vsOut.c1 = k * ((dot(F, P)) + (dot(F, camPos)));
	vsOut.c2 = (1 - (2 * k) * dot(F, P));
	vsOut.F_DOT_V = dot(F, V);

	vsOut.pos = mul(PV, P);
	vsOut.color = fogData[id].color;

	return vsOut;
}

Texture2D worldPosMap : register(t1);

float4 PS (VSOut psIn) : SV_Target
{
	//float3 worldPos = worldPosMap.Load(psIn.pos.xyz).xyz;

	float3 fogColor = (0.4 ,0.4, 0.4);

	float g = min(psIn.c2, 0.0);

	g = -length(psIn.aV) * (psIn.c1 - g * g / abs(psIn.F_DOT_V));

	float f = saturate(exp2(-g));

	psIn.color.rgb = psIn.color.rgb * f + fogColor.rgb * (1.0 - f);

	return float4(psIn.color.rgb, 0.2);
}