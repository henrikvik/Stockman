#include "include/Camera.hlsli"
#include "include/LightCalc.hlsli"

cbuffer cb0 : register(b0) { Camera camera; };
cbuffer cb1 : register(b1) { DirectionalLight globalLight; };

SamplerState           LinearClamp       : register(s0);
SamplerState           LinearWrap        : register(s1);
SamplerComparisonState ComparisonSampler : register(s2);

Texture2D              Position          : register(t3);
Texture2D              AlbedoSpecular    : register(t4);
Texture2D              Normal            : register(t5);
Texture2D              ShadowMap         : register(t6);
Texture2D              GlowTexture       : register(t7);

struct VSOutput
{
    float4 position : SV_Position;
    float2 uv       : UV;
};

VSOutput VS(uint id: SV_VertexID)
{
    VSOutput vsout;

    vsout.uv = float2((id << 1) & 2, id & 2);
    vsout.position = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);

    return vsout;
}

float4 PS(VSOutput input) : SV_Target0
{
    int2 uv = int2(input.position.xy);

    float3 position = Position.Load(int3(uv, 0)).xyz;
    float3 normal = Normal.Load(int3(uv, 0)).xyz;
    float4 albedoSpecular = AlbedoSpecular.Load(int3(uv, 0));

    float shadowFactor = calcShadowFactor(ComparisonSampler, ShadowMap, globalLight, 2);
    float3 viewDir = normalize(camera.position.xyz - input.position.xyz);

    float3 lightSum = float3(0, 0, 0);
    lightSum += calcLight(globalLight, position, normal, viewDir, albedoSpecular.w);
    lightSum += calcAllLights(input.position, position, normal, viewDir, albedoSpecular.w);

    return float4(lightSum * albedoSpecular.rgb, 1);
}