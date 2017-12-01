#include "include/Camera.hlsli"
#include "include/LightCalc.hlsli"
#include "include/Particles.hlsli"

cbuffer cb0 : register(b0) { Camera camera; };
cbuffer cb1 : register(b1) { DirectionalLight globalLight; };

Texture2D Noise : register(t5);
SamplerState Sampler : register(s1);

float4 PS(PSInput input) : SV_Target0
{
    float noise = Noise.SampleLevel(Sampler, input.noisescale*input.uv + float2(0, input.age * input.noisespeed), 0).g;
    float cap = 1 - input.age;

    clip(
        saturate(
            step(0.13, (noise*cap*0.95))
        ) - 0.005
    );

    float t = abs(input.deform*input.uv.y) + 0.04;
    float falloff = (1 - input.age);

    float3 diffuse = input.color.xyz;

    float3 viewDir = normalize(camera.position - input.worldPos);
    float3 lights = GLOBAL_LIGHT_COLOR;
    lights += calcLight(globalLight, input.worldPos, input.normal, viewDir, 0.1);
    lights += calcAllLights(input.position, input.worldPos, input.normal, viewDir, 0.1);

    return float4(diffuse * lights, 1);
}

void PS_depth(PSInput input)
{
    float noise = Noise.SampleLevel(Sampler, input.noisescale*input.uv + float2(0, input.age * input.noisespeed), 0).g;
    float cap = 1 - input.age;

    clip(
        saturate(
            step(0.13, (noise*cap*0.95))
        ) - 0.005
    );
}