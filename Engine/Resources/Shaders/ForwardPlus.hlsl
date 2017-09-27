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

cbuffer LightBuffer : register(b1)
{
    float4 dirLightPos;
    float3 dirLightColor;
    float dirFade;
}

cbuffer LightMatBuffer : register(b2)
{
    float4x4 lightVP;
}

struct InstanceData
{
    float4x4 world;
};
StructuredBuffer<InstanceData> instanceData : register(t20);
cbuffer InstanceOffsetBuffer : register(b3)
{
    uint instanceOffset;
}

struct Light 
{
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
};

struct VSOutput {
	float4 pos : SV_POSITION;
	float4 worldPos : POS;
    float4 lightPos : LIGHT_POS;
	float3 normal : NORMAL;
	float2 uv : UV;
    float3 biTangent : BITANGENT;
    float3 tangent : TANGENT;
};

VSOutput VS(VSInput input, uint instanceId : SV_InstanceId) {
	VSOutput output;

    float4x4 world = instanceData[instanceOffset + instanceId].world;

	output.worldPos = mul(world, float4(input.position, 1));
    output.pos = mul(ViewProjection, output.worldPos);

	output.uv = input.uv;
    output.normal = mul(world, float4(input.normal, 0));
    output.normal = normalize(output.normal);

    output.lightPos = output.worldPos + float4(output.normal * 0.15f, 0);
    output.lightPos = mul(lightVP, output.lightPos);

    output.tangent.xy = input.tangent.xy;
    output.biTangent.xy = input.biTangent.xy;


    output.tangent.z = sqrt(1 - pow(output.tangent.x, 2) + pow(output.tangent.y, 2));
    output.biTangent.z = sqrt(1 - pow(output.biTangent.x, 2) + pow(output.biTangent.y, 2));


    output.biTangent = normalize(mul(world, output.biTangent));
    output.tangent = normalize(mul(world, output.tangent));

	return output;
}

StructuredBuffer<uint> LightIndexList : register(t0);
Texture2D<uint2> LightGrid : register(t1);
StructuredBuffer<Light> Lights : register(t2);

Texture2D shadowMap : register(t3);
SamplerState Sampler : register(s0);

SamplerComparisonState cmpSampler : register(s1);

Texture2D diffuseMap : register(t10);
Texture2D normalMap : register(t11);
Texture2D specularMap : register(t12);

struct PSOutput {
	float4 color : SV_Target;
};

//Returns the shadow amount of a given position
float getShadowValue(float4 lightPos, int sampleCount = 1)
{
    lightPos.x = (lightPos.x * 0.5f) + 0.5f;
    lightPos.y = (lightPos.y * -0.5f) + 0.5f;

    float addedShadow = 0;

    for (int y = -sampleCount; y <= sampleCount; y += 1)
    {
        for (int x = -sampleCount; x <= sampleCount; x += 1)
        {
            addedShadow += shadowMap.SampleCmp(cmpSampler, lightPos.xy, lightPos.z, int2(x, y)).r;
        }
    }

    float shadow = addedShadow / pow(sampleCount * 2 + 1, 2);

    return shadow;
}

[earlydepthstencil]
PSOutput PS(VSOutput input) {
	PSOutput output;

	uint2 tile = uint2(floor(input.pos.xy / BLOCK_SIZE));
	uint offset = LightGrid[tile].x;
	uint count = LightGrid[tile].y;

    float3 colorSample = diffuseMap.Sample(Sampler, input.uv);
    float3 specularSample = specularMap.Sample(Sampler, input.uv);
    float3 normalSample = normalMap.Sample(Sampler, input.uv);

    /////////////////NORMAL MAPPING
    //To make sure the tangent is perpendicular
    input.tangent = normalize(input.tangent - dot(input.tangent, normalSample) * normalSample);

    float3x3 tangentMatrix = float3x3(input.tangent, input.biTangent, input.normal);
    normalSample = normalize(normalSample * 2.0 - 1);


    float3 finalNormal = normalize(mul(normalSample, tangentMatrix));

    /////////////////////////////////////////////////////////
	

    ///////////////////////////////DIRECTIONAL LIGHT///////////////////////////////////////
    float3 lightDir = normalize(camPos.xyz - dirLightPos.xyz);
    float diffuseFactor = saturate(dot(finalNormal, normalize(-lightDir)));
    float3 directionalDiffuse = diffuseFactor * dirLightColor;

    float3 posToLightDir = dirLightPos.xyz - input.worldPos.xyz;
    float3 reflectThingDir = normalize(posToLightDir + (camPos.xyz - input.worldPos.xyz));
    float3 directionalSpecularity = pow(saturate(dot(finalNormal, reflectThingDir)), 500) * dirLightColor;

    
    float shadow = getShadowValue(input.lightPos, 2);

    directionalDiffuse *= dirFade * shadow;
    directionalSpecularity *= dirFade * shadow;

    /////////////////////////////DIRECTIONAL LIGHT END///////////////////////////////////////


    ////////////////////////////////POINT LIGHTS//////////////////////////////////////////////


    float3 pointDiffuse = 0;
    float3 pointSpecular = 0;

	for (uint i = 0; i < count; i++) 
    {
		uint idx = LightIndexList[offset + i];
		Light light = Lights[idx];

		float3 posToLight = light.positionWS - input.worldPos.xyz;
        float3 reflectThing = normalize(posToLight + (camPos.xyz - input.worldPos.xyz));

        float distance = length(posToLight);
        float3 normalizedLight = posToLight / distance;
		float attenuation = 1.0f - smoothstep(0, light.range, distance);

        pointDiffuse += saturate(dot(finalNormal, posToLight)) * light.color * attenuation;

        pointSpecular += pow(saturate(dot(finalNormal, reflectThing)), 1000) * light.color * attenuation;
    }
    //////////////////////////////POINT LIGHTS END//////////////////////////////////////////////

    float3 finalDiffuse = saturate(pointDiffuse + directionalDiffuse) * colorSample;
    float3 finalSpecular = saturate(pointSpecular + pointDiffuse) * specularSample;
    
    float3 ambient = float3(0.2, 0.2, 0.3) * colorSample;

    float3 lighting = saturate(finalDiffuse + finalSpecular + ambient);
    
    output.color = float4(lighting, 1);

        return output;
}

