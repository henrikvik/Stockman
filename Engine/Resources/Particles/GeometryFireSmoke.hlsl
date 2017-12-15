#include "include/Camera.hlsli"
#include "include/LightCalc.hlsli"
#include "include/Particles.hlsli"

cbuffer cb0 : register(b0) { Camera camera; };
cbuffer cb1 : register(b1) { DirectionalLight globalLight; };

Texture2D Noise : register(t5);
SamplerState Sampler : register(s1);

float4 PS(PSInput input) : SV_Target0
{
    float noise = Noise.SampleLevel(Sampler, input.noisescale*input.uv + float2(0, input.noisespeed), 0).g;
    float cap = 1 - input.age;

    float4x4 thresholdMatrix =
    {
        1.0 / 17.0,  9.0 / 17.0,  3.0 / 17.0, 11.0 / 17.0,
        13.0 / 17.0,  5.0 / 17.0, 15.0 / 17.0,  7.0 / 17.0,
        4.0 / 17.0, 12.0 / 17.0,  2.0 / 17.0, 10.0 / 17.0,
        16.0 / 17.0,  8.0 / 17.0, 14.0 / 17.0,  6.0 / 17.0
    };

    float dist = 1 - saturate(distance(input.worldPos, camera.position) / 5.f);
    float closeness = thresholdMatrix[input.position.x % 4][input.position.y % 4] * dist;

    clip(
        saturate(
            step(0.13, (noise*cap*0.95))
        ) - 0.005 - closeness * 14
    );;
    
    float t = abs(input.deform*input.uv.y)+0.04;
    float falloff = (1 - input.age);

    float innerFactor = step(input.age*1.85 - 0.01, (0.55-input.age*0.15)*t*noise *3);
    float outerFactor = 1 - innerFactor;

    float3 inner = float3(1, 0.6, 0.3)*2;
    float3 innerEnd = inner;

    float3 outer = input.color.xyz;
    float3 outerEnd = input.color.xyz;

    float3 diffuse = ((innerFactor * lerp(inner, innerEnd, falloff - 0.49)) + (outerFactor * lerp(outerEnd, outer, 1- pow(input.deform, 8))));

    float3 viewDir = normalize(camera.position - input.worldPos);
    float3 lights = globalLight.ambient;
    lights += calcLight(globalLight, input.worldPos, input.normal, viewDir, 0.1);
    lights += calcAllLights(input.position, input.worldPos, input.normal, viewDir, 0.1);


    float3 color = diffuse * lights;
    color = lerp(color, FOG_COLOR, saturate(distance(camera.position.xyz, input.worldPos.xyz) / 130.f));

    return float4(color, 1);
}

void PS_depth(PSInput input)
{
    float noise = Noise.SampleLevel(Sampler, input.noisescale*input.uv + float2(0, input.noisespeed), 0).g;
    float cap = 1 - input.age;

    float4x4 thresholdMatrix =
    {
        1.0 / 17.0,  9.0 / 17.0,  3.0 / 17.0, 11.0 / 17.0,
        13.0 / 17.0,  5.0 / 17.0, 15.0 / 17.0,  7.0 / 17.0,
        4.0 / 17.0, 12.0 / 17.0,  2.0 / 17.0, 10.0 / 17.0,
        16.0 / 17.0,  8.0 / 17.0, 14.0 / 17.0,  6.0 / 17.0
    };

    float dist = 1 - saturate(distance(input.worldPos, camera.position) / 5.f);
    float closeness = thresholdMatrix[input.position.x % 4][input.position.y % 4] * dist;

    clip(
        saturate(
            step(0.13, (noise*cap*0.95))
        ) - 0.005 - closeness * 14
    );
}