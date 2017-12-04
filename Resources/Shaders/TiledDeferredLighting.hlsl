#include "include/Camera.hlsli"
#include "include/LightCalc.hlsli"

cbuffer cb0 : register(b0) { Camera camera; };
cbuffer cb1 : register(b1) { DirectionalLight globalLight; };
cbuffer ShadowBuffer : register(b2)
{
    float4x4 ShadowVP;
    float4x4 InvShadowVP;
};


SamplerState           LinearClamp       : register(s0);
SamplerState           LinearWrap        : register(s1);
SamplerComparisonState ComparisonSampler : register(s2);

Texture2D              Position          : register(t3);
Texture2D              AlbedoSpecular    : register(t4);
Texture2D              Normal            : register(t5);
Texture2D              ShadowMap         : register(t6);
Texture2D              AO                : register(t7);

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

struct Targets {
    float4 Backbuffer : SV_Target0;
    float4 Glow : SV_Target1;
};

Targets PS(VSOutput input)
{
    Targets targets;

    int2 uv = int2(input.position.xy);

    float3 position = Position.Load(int3(uv, 0)).xyz;
    float3 normal = Normal.Load(int3(uv, 0)).xyz;
    float4 albedoSpecular = AlbedoSpecular.Load(int3(uv, 0));
    float ao = AO.Load(int3(uv, 0)).r;

    float shadowFactor = calcShadowFactor(ComparisonSampler, ShadowMap, mul(InvShadowVP, float4(position, 1.0)).xyz);
    float3 viewDir = normalize(camera.position.xyz - position.xyz);

    float3 lightSum = ao * globalLight.ambient;
    lightSum += shadowFactor * calcLight(globalLight, position, normal, viewDir, albedoSpecular.w);
    lightSum += calcAllLights(input.position, position, normal, viewDir, albedoSpecular.w);

    float3 color = float3(0, 0, 0);
    //if (fragment.useGridTexture)
    {
    //    color = gridTexture.Sample(linearWrap, fragment.gridUV).xyz;
    }
    //else
    {
        color = albedoSpecular.xyz;
    }

    //clip(diffuseTexture.Sample(linearClamp, fragment.uv).a - 0.5.);
    targets.Backbuffer = float4(lightSum * color, 1);

    float3 glow = targets.Backbuffer.rgb;
    float luminance = dot(float3(0.2126, 0.7152, 0.0722), glow);
    if (luminance > 1.0) {
        targets.Glow = float4(glow, 0);
    }
    else {
        targets.Glow = float4(0, 0, 0, 0);
    }

    targets.Backbuffer = float4(ao, ao, ao, 1);
    return targets;
}