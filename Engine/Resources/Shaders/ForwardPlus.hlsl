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
    float4x4 dummy;
    float4 camPos;
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
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float2 biTangent : BITANGENT;
	float2 tangent : TANGENT;

	// INSTANCE DATA
	float4x4 world : WORLD;
};

struct VSOutput {
	float4 pos : SV_POSITION;
	float4 worldPos : POS;
	float3 normal : NORMAL;
	float2 uv : UV;
};

VSOutput VS(VSInput input) {
	VSOutput output;

	output.worldPos = mul(input.world, float4(input.position, 1));
	output.pos = mul(ViewProjection, output.worldPos);

	output.uv = input.uv;
    output.normal = mul(input.world, float4(input.normal, 0));
    output.normal = normalize(output.normal);
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
	
	// temp WHEN DAY/NIGHT CYCLE IS A THING THIS WILL BE REMOVED
    float3 lightDir = float3(0, -0.5, 0.5);
    float diffuseFactor = saturate(dot(input.normal, normalize(-lightDir)));
    float3 directionalDiffuse = diffuseFactor * float3(0.1, 0.4, 0.7);


    float3 ambient = float3(0, 0.1, 0.2);

    //This will include a texture sample or material color later
	float3 color = 0.1f;

	for (uint i = 0; i < count; i++) 
    {
		uint idx = LightIndexList[offset + i];
		Light light = Lights[idx];

		float3 posToLight = light.positionWS - input.worldPos.xyz;
        float3 reflectThing = normalize(posToLight + (camPos.xyz - input.worldPos.xyz));

        float distance = length(posToLight);
        float3 normalizedLight = posToLight / distance;
		float attenuation = 1.0f - smoothstep(0, light.range, distance);

        float3 diffuse = saturate(dot(input.normal, posToLight));

       
        float3 specular = pow(saturate(dot(input.normal, reflectThing)), 1000) * light.color;
        color += light.color * attenuation * (saturate(diffuse) + specular);
	}

    output.color = float4(saturate(directionalDiffuse + color), 1);
    //output.color = float4(input.normal.xyz, 1);
	return output;
}