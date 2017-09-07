/*
*  Author: fkaa
*
*/

#define NUM_LIGHTS 8

cbuffer Camera : register(b0)
{
	float4x4 ViewProjection;
	float4x4 InvProjection;
}

// TODO: change
struct Light {
	float3 position;
	float range;
	float3 color;
	float intensity;
};

StructuredBuffer<Light> Lights : register(t2);

struct VSInput {
	float3 pos : POS;
};

struct VSOutput {
	float4 pos : SV_POSITION;
};

VSOutput VS(VSInput input) {
	VSOutput output;

	output.pos = mul(ViewProjection, float4(input.pos, 1.0));

	return output;
}

struct PSOutput {
	float4 color : SV_Target;
};

PSOutput PS(VSOutput input) {
	PSOutput output;

	output.color = float4(1, 0, 0, 1);

	return output;
}