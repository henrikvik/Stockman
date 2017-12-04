#include "ShaderConstants.hlsli"
#include "LightCalcInclude.hlsli"

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VSOut VS(uint id: SV_VertexID)
{
    VSOut vsout;
    
	vsout.uv = float2((id << 1) & 2, id & 2);
	vsout.pos = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);

    return vsout;
}

cbuffer FXConstants : register(b0)
{
    float BulletTimeProgress;
}

Texture2D Backbuffer : register(t0);
Texture2D GlowTexture : register(t1);
Texture2D SSSAOTexture : register(t2);

SamplerState LinearClamp : register(s0);

float4 PS(VSOut input) : SV_Target0
{
    float3 color = Backbuffer.Load(int3(input.pos.xy, 0));
    float3 glow = float3(0, 0, 0);

    glow += 0.4 * GlowTexture.SampleLevel(LinearClamp, input.uv, 0).rgb;
    glow += 0.1 * GlowTexture.SampleLevel(LinearClamp, input.uv, 1).rgb;
    glow += 0.5 * GlowTexture.SampleLevel(LinearClamp, input.uv, 2).rgb;

    color = color.rgb * SSSAOTexture.Sample(LinearClamp, input.uv).r;

    color = adjustSaturation(color, GLOW_ORIGINAL_SATURATION) * GLOW_ORIGINAL_INTENSITY;
    glow = adjustSaturation(glow, GLOW_SATURATION) * GLOW_INTENSITY;

    color = color + color * glow * 7;

    color = adjustContrast(color, 2 - BulletTimeProgress, 0.3f);
    color = adjustSaturation(color, BulletTimeProgress);

    return float4(saturate(color), 1);
}