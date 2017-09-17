/*
*  Author: fkaa
*
*/

#define BLOCK_SIZE 16.f
#define NUM_LIGHTS 8

cbuffer Camera : register(b0)
{
	float4x4 ViewProjection;
	float4x4 InvProjection;
}

// TODO: change
struct Light {
	float4 positionVS;
	float3 positionWS;
	float range;
	float3 color;
	float intensity;
};

struct VSInput
{
	// Vertex data
	float3 pos : POS;
	float2 uv : UV;
	float3 normal : NORMAL;
	uint material : MATERIAL;

	float3 offset : OFFSET;
};

struct VSOutput {
	float4 pos : SV_POSITION;
	float4 worldPos : POS;
	float3 normal : NORMAL;
	float2 uv : UV;
};

VSOutput VS(VSInput input) {
	VSOutput output;
	
	output.pos = float4(input.pos, 1);
	output.pos.xyz += input.offset.xyz;
	output.pos = mul(ViewProjection, output.pos);
	
	output.worldPos = float4(input.pos, 1.0);
	output.worldPos.xyz += input.offset.xyz;

	output.uv = input.uv;
	output.normal = input.normal;

	return output;
}

StructuredBuffer<uint> LightIndexList : register(t0);
Texture2D<uint2> LightGrid : register(t1);
StructuredBuffer<Light> Lights : register(t2);

Texture2D Texture : register(t3);
SamplerState Sampler : register(s0);

struct PSOutput {
	float4 color : SV_Target;
};

[earlydepthstencil]
PSOutput PS(VSOutput input) {
	PSOutput output;

	uint2 tile = uint2(floor(input.pos.xy / BLOCK_SIZE));
	uint offset = LightGrid[tile].x;
	uint count = LightGrid[tile].y;

	float3 diffuse = Texture.Sample(Sampler, input.uv).xyz;
	float3 color = float3(0.5, 0.5, 0.5);
	for (uint i = 0; i < count; i++) {
		uint idx = LightIndexList[offset + i];
		Light light = Lights[idx];


		float3 L = light.positionWS - input.worldPos.xyz;
		float distance = length(L);
		L = L / distance;
		float d = 1.0f - smoothstep(light.range * 0.5f, light.range, distance);
		color += light.color * saturate(d);
	}

	output.color = float4(diffuse * color, 1);

	return output;
}