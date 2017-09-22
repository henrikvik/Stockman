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
    float4 lightPos : LIGHT_POS;
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

    output.lightPos = output.worldPos + float4(output.normal * 0.18f, 0);
    output.lightPos = mul(lightVP, output.lightPos);

	return output;
}

StructuredBuffer<uint> LightIndexList : register(t0);
Texture2D<uint2> LightGrid : register(t1);
StructuredBuffer<Light> Lights : register(t2);

Texture2D shadowMap : register(t3);
SamplerState Sampler : register(s0);

SamplerComparisonState cmpSampler : register(s1)
{
   // sampler state
    Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
    AddressU = MIRROR;
    AddressV = MIRROR;
 
   // sampler comparison state
    ComparisonFunc = LESS_EQUAL;
};

struct PSOutput {
	float4 color : SV_Target;
};

[earlydepthstencil]
PSOutput PS(VSOutput input) {
	PSOutput output;

	uint2 tile = uint2(floor(input.pos.xy / BLOCK_SIZE));
	uint offset = LightGrid[tile].x;
	uint count = LightGrid[tile].y;
	

    ///////////////////////////////DIRECTIONAL LIGHT///////////////////////////////////////
    float3 lightDir = normalize(camPos.xyz - dirLightPos.xyz);
    float diffuseFactor = saturate(dot(input.normal, normalize(-lightDir)));
    float3 directionalDiffuse = diffuseFactor * dirLightColor;

    float3 posToLightDir = dirLightPos.xyz - input.worldPos.xyz;
    float3 reflectThingDir = normalize(posToLightDir + (camPos.xyz - input.worldPos.xyz));
    float3 specularDir = pow(saturate(dot(input.normal, reflectThingDir)), 500) * dirLightColor;

    

            /////////////////////////////SHADOWS//////////////////////////////
    input.lightPos.x = (input.lightPos.x * 0.5f) + 0.5f;
    input.lightPos.y = (input.lightPos.y * -0.5f) + 0.5f;

    float addedShadow = 0;

    int samples = 1;

    for (int y = -samples; y <= samples; y += 1)
    {
        for (int x = -samples; x <= samples; x += 1)
        {
            addedShadow += shadowMap.SampleCmp(cmpSampler, input.lightPos.xy, input.lightPos.z, int2(x, y)).r;
        }
    }

    float shadow = addedShadow / pow(samples * 2 + 1, 2);
    
            //////////////////////// END SHADOW /////////////////////////

    float3 directionalComponent = (directionalDiffuse + specularDir) * dirFade * shadow;

    /////////////////////////////DIRECTIONAL LIGHT END///////////////////////////////////////


    float3 ambient = float3(0.1, 0.1, 0.2);



    //This will include a texture sample or material color later
	float3 matColor = 0.1f;

    float3 pointLightComponent = 0;



    ////////////////////////////////POINT LIGHTS//////////////////////////////////////////////
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
        pointLightComponent += light.color * attenuation * (saturate(diffuse) + specular);
	}
    //////////////////////////////POINT LIGHTS END//////////////////////////////////////////////




    output.color = float4(saturate(directionalComponent + pointLightComponent + ambient), 1);
   

    //DEBUG TEST TEMP
    //if (input.lightPos.x > 1 || input.lightPos.x < 0 ||
    //    input.lightPos.y > 1 || input.lightPos.y < 0)
    //    output.color.xyz = ambient;

        


	return output;
}