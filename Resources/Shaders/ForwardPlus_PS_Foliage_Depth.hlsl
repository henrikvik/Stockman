#include "include/Fragment.hlsli"

SamplerState           linearClamp     : register(s0);
Texture2D              diffuseTexture  : register(t12);


void PS(Fragment fragment)
{
    float4 diffuse = diffuseTexture.Sample(linearClamp, fragment.uv);
    clip(diffuse.a - 0.5);
}