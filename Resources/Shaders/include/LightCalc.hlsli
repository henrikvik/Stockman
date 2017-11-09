//If you want to include this file this is the allocated resources:
//cbuffer register 0, 1, 2 and 3
//0: Camera     1: DirectionalLight     2: LightVP

//texture 0, 1, 2, 3, 10, 11, 12 and 13
//0: LightIndexList     1: LightGrid    2: lights   3: shadowmap
//10: diffuseMap    11: NormalMap   12: specularMap     13: glowMap

//samplerState 0 and 1
//0: linear     1: comparisonSomething

//Use other registers for YOUR resources.


#define BLOCK_SIZE 16.f
#define GLOBAL_LIGHT_COLOR float3(0.1, 0.1, 0.3)

struct DirectionalLight
{
    float4 position;
    float3 color;
    float3 ambient;
};

struct Light
{
    float4 positionVS;
    float3 positionWS;
    float range;
    float3 color;
    float intensity;
};

//makes stuff gray
float3 adjustSaturation(float3 color, float saturation)
{
    //Touch these values and yer ded kid. (lower saturation or something)
    float grey = dot(color, float3(0.3, 0.59, 0.11));
    return lerp(grey, color, saturation);
}

float3 adjustContrast(float3 color, float contrast, float threshold)
{
    return (color - threshold) * max(contrast, 0.f) + threshold;
}

//Returns the shadow amount of a given position
float calcShadowFactor(SamplerComparisonState comparisonSampler, Texture2D shadowMap, DirectionalLight light, int sampleCount)
{
    light.position.x = (light.position.x * 0.5f) + 0.5f;
    light.position.y = (light.position.y * -0.5f) + 0.5f;

    float addedShadow = 0;

    [unroll]
    for (int y = -sampleCount; y <= sampleCount; y += 1)
    {
        [unroll]
        for (int x = -sampleCount; x <= sampleCount; x += 1)
        {
            addedShadow += shadowMap.SampleCmp(comparisonSampler, light.position.xy, light.position.z, int2(x, y)).r;
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



/*
//Specularity is currently broken. 
float3 calculateSpecularity(float3 specularColor, float3 wPos, float3 lightPos, float3 NDCPos, float2 uv, float3 normal, float shadowValue = 1)
{
    uint2 tile = uint2(floor(NDCPos.xy / BLOCK_SIZE));
    uint offset = LightGrid[tile].x;
    uint count = LightGrid[tile].y;
    
    float3 dirColor = float3(0.1, 0.1, 0.3); //getCurrentDirColor(1 - fade);
        
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
    
    float4 ambient = float4(0.05, 0.05, 0.05, 1.0) * colorSample;

    float4 lighting = saturate(finalDiffuse + ambient);
    
    return lighting;
}
*/

float3 calcNormal(float3 mappedNormal, float3 normal, float3 binormal, float3 tangent)
{
    // TODO Remove when everything is working
    if (mappedNormal.x == 0 && mappedNormal.y == 0 && mappedNormal.z == 0)
        return normal;
    if (mappedNormal.x == 1 && mappedNormal.y == 1 && mappedNormal.z == 1)
        return normal;
    ////

    //To make sure the tangent is perpendicular
    tangent = normalize(tangent - dot(tangent, mappedNormal) * mappedNormal);

    float3x3 tangentMatrix = float3x3(tangent, binormal, normal);
    mappedNormal = normalize(mappedNormal * 2.0 - 1);

    return normalize(mul(mappedNormal, tangentMatrix));
}

float3 calcDiffuse(DirectionalLight light, float4 position, float3 normal)
{
    float3 lightDir = normalize(light.position);
    return saturate(dot(normal, lightDir)) * GLOBAL_LIGHT_COLOR; // TODO USE light.color
}

float3 calcDiffuse(Light light, float4 position, float3 normal)
{
    float3 lightDir = light.positionWS - position.xyz;

    float distance = length(lightDir);
    float3 normalizedLight = lightDir / distance;
    float attenuation = 1.0f - smoothstep(0, light.range, distance);

    return saturate(dot(normal, lightDir)) * light.color * attenuation * light.intensity;
}

float3 calcAllLights(
    StructuredBuffer<uint> lightIndexList,
    Texture2D<uint2> lightGrid,
    StructuredBuffer<Light> lights,
    float4 ndcPosition,
    float4 position, 
    float3 normal
)
{
    float3 lightSum = float3(0, 0, 0);

    uint2 tile = uint2(floor(ndcPosition.xy / BLOCK_SIZE));
    uint offset = lightGrid[tile].x;
    uint count = lightGrid[tile].y;
    for (uint i = 0; i < count; i++)
    {
        uint idx = lightIndexList[offset + i];
        Light light = lights[idx];

        lightSum += calcDiffuse(light, position, normal);
    }

    return lightSum;
}


//TODO GÖR EN BÅDA //