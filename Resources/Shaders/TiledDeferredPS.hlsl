#include "include/Camera.hlsli"
#include "include/Fragment.hlsli"
#include "include/LightCalc.hlsli"

#define USE_GRID_TEXTURE

SamplerState           linearClamp     : register(s0);
SamplerState           linearWrap      : register(s1);
SamplerComparisonState comparison      : register(s2);

#ifdef USE_GRID_TEXTURE
Texture2D              gridTexture     : register(t4);
#endif

Texture2D              diffuseTexture  : register(t12);
Texture2D              normalTexture   : register(t13);
Texture2D              specularTexture : register(t14);
Texture2D              glowTexture     : register(t15);

struct RenderTargets
{
    float4 Position       : SV_Target0;
    float4 AlbedoSpecular : SV_Target1;
    float4 Normal         : SV_Target2;
};

RenderTargets PS(Fragment fragment)
{
    RenderTargets targets;

    float3 albedo = float3(0, 0, 0);
    if (fragment.useGridTexture) {
        albedo = gridTexture.Sample(linearWrap, fragment.gridUV).xyz;
    } else {
        albedo = diffuseTexture.Sample(linearClamp, fragment.uv).xyz;
    }
    float specular = specularTexture.Sample(linearClamp, fragment.uv).r;
    float3 normal = calcNormal(normalTexture.Sample(linearClamp, fragment.uv).xyz, fragment.normal, fragment.binormal, fragment.tangent);
    float3 glow = glowTexture.Sample(linearClamp, fragment.uv).xyz;

    targets.Position = float4(fragment.position);
    targets.AlbedoSpecular = float4(albedo + glow * 7, specular);
    targets.Normal = float4(normal, 0.0);

    return targets;
}