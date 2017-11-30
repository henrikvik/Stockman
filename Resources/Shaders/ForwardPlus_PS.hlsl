#include "include/Camera.hlsli"
#include "include/Fragment.hlsli"
#include "include/LightCalc.hlsli"

#define USE_GRID_TEXTURE

cbuffer cb0 : register(b0) { Camera camera; };
cbuffer cb1 : register(b1) { DirectionalLight globalLight; };

SamplerState           linearClamp     : register(s0);
SamplerState           linearWrap      : register(s1);
SamplerComparisonState comparison      : register(s2);

Texture2D              shadowMap       : register(t3);
#ifdef USE_GRID_TEXTURE
Texture2D              gridTexture     : register(t4);
#endif

Texture2D              diffuseTexture  : register(t12);
Texture2D              normalTexture   : register(t13);
Texture2D              specularTexture : register(t14);
Texture2D              glowTexture     : register(t15);

struct Targets
{
    float4 color      : SV_Target0;
    float4 glow       : SV_Target1;
    float4 viewNormal : SV_Target2;
};

Targets PS(Fragment fragment) 
{
    Targets targets;
    
    float3 normal = calcNormal(normalTexture.Sample(linearClamp, fragment.uv).xyz, fragment.normal, fragment.binormal, fragment.tangent);
    float specularExponent = specularTexture.Sample(linearClamp, fragment.uv).r;

    float shadowFactor = calcShadowFactor(comparison, shadowMap, fragment.lightPos);
    float3 viewDir = normalize(camera.position.xyz - fragment.position.xyz);

    float3 lightSum = globalLight.ambient;
    lightSum += shadowFactor * calcLight(globalLight, fragment.position.xyz, normal, viewDir, specularExponent);
    lightSum += calcAllLights(fragment.ndcPosition, fragment.position.xyz, normal, viewDir, specularExponent);
    
    float3 color = float3(0, 0, 0);
    if (fragment.useGridTexture)
    {
        color = gridTexture.Sample(linearWrap, fragment.gridUV).xyz;
    }
    else
    {
        color = diffuseTexture.Sample(linearClamp, fragment.uv).xyz;
    }

	//clip(diffuseTexture.Sample(linearClamp, fragment.uv).a - 0.5.);

    targets.color = float4(lightSum * color * fragment.color, 1); //500~

    float3 glow = targets.color.rgb + 1.7 * glowTexture.Sample(linearClamp, fragment.uv);
    float luminance = dot(float3(0.2126, 0.7152, 0.0722), glow);
    if (luminance > 1.0) {
        targets.glow = float4(glow, 0);
    }
    else {
        targets.glow = float4(0, 0, 0, 0);
    }
    //targets.glow = targets.color + glowTexture.Sample(linearClamp, fragment.uv); //300~
    targets.viewNormal = fragment.viewNormal; //300~

    return targets;
}