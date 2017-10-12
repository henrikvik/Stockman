#define a 0.01 // Color Change Radious
#define TRAD 7 // Transparancy Radious
#define FOGCOLOR float3(0.3, 0.3, 0.6) //Fog Color

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
	float3 worldPos : WORLD;
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
	
	//F is the four dimensional vector that represents the bounding plane, XYZ 
	//is the normal of the plane. 
	float4 F = float4(0,-1, 0, 1);
	F = normalize(F);
	//ViewVec
	float4 P = float4(fogData[id].pos, 1);
	float4 V = camPos - P;
	//a is a random posetive constant, NOTE if the value is over 20 or the color of
	//the fragment is going to "shine through" when under the fog halfspace
	//float a = 0.01;
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
	vsOut.worldPos = P.xyz;

	return vsOut;
}

Texture2D worldPosMap : register(t1);

float4 PS (VSOut psIn) : SV_Target
{
	float3 worldPos = worldPosMap.Load(float3(psIn.pos.xy, 0)).xyz;

	float playerDistance = length( psIn.worldPos.xyz - camPos.xyz);

	float3 fogColor = FOGCOLOR;

	float g = min(psIn.c2, 0.0);

	//calculates where the cam is and is used to let 
	g = -length(psIn.aV) * (psIn.c1 - g * g / abs(psIn.F_DOT_V));

	float f = saturate(exp2(-g));

	float fogDepth = clamp(min(saturate(1 - worldPos.y), saturate(playerDistance / TRAD)), 0, 0.8);

	psIn.color.rgb = psIn.color.rgb * f + fogColor.rgb * (1.0 - f);

	return float4(psIn.color.rgb, fogDepth);
}