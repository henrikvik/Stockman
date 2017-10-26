#include "ShaderConstants.hlsli"

//If you want to include this file this is the allocated resources:
//cbuffer register 0, 1, 2 and 3
//0: Camera     1: DirectionalLight     2: BulletTime   3: LightVP

//texture 0, 1, 2, 3, 10, 11, 12 and 13
//0: LightIndexList     1: LightGrid    2: lights   3: shadowmap
//10: diffuseMap    11: NormalMap   12: specularMap     13: glowMap

//samplerState 0 and 1
//0: linear     1: comparisonSomething

//Use other registers for YOUR resources.

#define BLOCK_SIZE 16.f
#define MAX_LIGHTS 128

//used by VS & PS
cbuffer Camera : register(b0)
{
    float4x4 ViewProjection;
    float4x4 InvProjection;
    float4x4 View;
    float4 camPos;
}

//used by PS
cbuffer LightBuffer : register(b1)
{
    float4 dirLightPos;
    float fade;
}

//Used by PS
cbuffer BulletTimeTimer : register(b2)
{
    float bulletTimer;
};

//used by VS
cbuffer LightMatBuffer : register(b3)
{
    float4x4 lightVP;
}

//Used by PS
struct Light
{
    float4 positionVS;
    float3 positionWS;
    float range;
    float3 color;
    float intensity;
};

#define DAY_COLOR float3(0.1, 0.1, 0.3)//float3(1, 1, 0.8)
#define DAWN_COLOR float3(0.1, 0.1, 0.3)//float3(2, 0.5, 0)
#define NIGHT_COLOR float3(0.1, 0.1, 0.3)

#define FREEZE_COLOR float3(0.3, 0.6, 1)
#define BURN_COLOR float3(1, 0.2, 0)


StructuredBuffer<uint> LightIndexList : register(t0);
Texture2D<uint2> LightGrid : register(t1);
StructuredBuffer<Light> Lights : register(t2);

Texture2D shadowMap : register(t3);
SamplerState Sampler : register(s0);

SamplerComparisonState cmpSampler : register(s1);

Texture2D diffuseMap : register(t10);
Texture2D normalMap : register(t11);
Texture2D specularMap : register(t12);
Texture2D glowMap : register(t13);

//Returns the shadow amount of a given position
float calculateShadowValue(float3 lightPos, int sampleCount = 1)
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

//This is because i waas bored, it is not something we will use maybe i think
float3 toonify(float3 color, float intensity)
{
    if (intensity > 0.95)
        color = color; //Yes.
    else if (intensity > 0.5)
        color = 0.7 * color;
    else if (intensity > 0.05)
        color = 0.35 * color;
    else
        color = 0.1 * color;
    
    return color;
}

float3 getNormalMappedNormal(float3 tangent, float3 biTangent, float3 normal, float2 uv)
{
    float3 normalSample = normalMap.Sample(Sampler, uv);

    //Remove when everything is working
    if (normalSample.x == 0 && normalSample.y == 0 && normalSample.z == 0)
        return normal;

    //To make sure the tangent is perpendicular
    tangent = normalize(tangent - dot(tangent, normalSample) * normalSample);

    float3x3 tangentMatrix = float3x3(tangent, biTangent, normal);
    normalSample = normalize(normalSample * 2.0 - 1);


    return normalize(mul(normalSample, tangentMatrix));
}

float3 getCurrentDirColor(float time)
{
    float3 dirColor0 = lerp(DAY_COLOR, DAWN_COLOR, time);
    float3 dirColor1 = lerp(DAWN_COLOR, NIGHT_COLOR, time);
    float3 dirColor2 = lerp(dirColor0, dirColor1, time);

    return dirColor2;
}

float3 calculateStatusEffect(float3 color, float freeze, float burn)
{
    color = lerp(color, FREEZE_COLOR, freeze);
    color += lerp(color, BURN_COLOR, burn);

    return color;
}

//Specularity is currently broken.
float3 calculateSpecularity(float3 wPos, float3 lightPos, float3 NDCPos, float2 uv, float3 normal, float shadowValue = 1)
{
    uint2 tile = uint2(floor(NDCPos.xy / BLOCK_SIZE));
    uint offset = LightGrid[tile].x;
    uint count = LightGrid[tile].y;
    
    float3 dirColor = getCurrentDirColor(1 - fade);

    float3 specularSample = specularMap.Sample(Sampler, uv);
    
    float3 posToLightDir = dirLightPos.xyz - wPos.xyz;
    float3 reflectThingDir = normalize(posToLightDir + (camPos.xyz - wPos.xyz));
    float3 directionalSpecularity = pow(saturate(dot(normal, reflectThingDir)), 500) * dirColor;
    
    directionalSpecularity *= fade * shadowValue;

    float3 pointSpecular = 0;

    for (uint i = 0; i < count; i++)
    {
        uint idx = LightIndexList[offset + i];
        Light light = Lights[idx];

        float3 posToLight = light.positionWS - wPos.xyz;
        float3 reflectThing = normalize(posToLight + (camPos.xyz - wPos.xyz));

        float distance = length(posToLight);
        float3 normalizedLight = posToLight / distance;
        float attenuation = 1.0f - smoothstep(0, light.range, distance);

        pointSpecular += pow(saturate(dot(normal, reflectThing)), 1000) * light.color * attenuation;
    }

    float3 finalSpecular = saturate(pointSpecular + directionalSpecularity) * specularSample;

    return finalSpecular;
}

float4 calculateDiffuseLight(float3 wPos, float3 lightPos, float3 NDCPos, float2 uv, float3 normal, float shadowValue = 1)
{
    uint2 tile = uint2(floor(NDCPos.xy / BLOCK_SIZE));
    uint offset = LightGrid[tile].x;
    uint count = LightGrid[tile].y;

    float3 dirColor = getCurrentDirColor(1 - fade);

    float4 colorSample = diffuseMap.Sample(Sampler, uv);

    float3 lightDir = normalize(camPos.xyz - dirLightPos.xyz);
    float diffuseFactor = saturate(dot(normal, normalize(-lightDir)));
    float3 directionalDiffuse = diffuseFactor * dirColor;

    directionalDiffuse *= fade * shadowValue;

    float3 pointDiffuse = 0;

    for (uint i = 0; i < count; i++)
    {
        uint idx = LightIndexList[offset + i];
        Light light = Lights[idx];

        float3 posToLight = light.positionWS - wPos.xyz;

        float distance = length(posToLight);
        float3 normalizedLight = posToLight / distance;
        float attenuation = 1.0f - smoothstep(0, light.range, distance);

        pointDiffuse += saturate(dot(normal, posToLight)) * light.color * attenuation * light.intensity;
    }

    float4 finalDiffuse = float4(saturate(pointDiffuse + directionalDiffuse).xyz, 1) * colorSample;
    
    float4 ambient = float4(0.2, 0.2, 0.3, 1.0) * colorSample;

    float4 lighting = saturate(finalDiffuse + ambient);
    
    lighting.xyz = adjustSaturation(lighting.xyz, bulletTimer);
    lighting.xyz = adjustContrast(lighting.xyz, 2 - bulletTimer, 0.3);

    return lighting;
}