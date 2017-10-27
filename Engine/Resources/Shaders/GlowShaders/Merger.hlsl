#include "../ShaderConstants.hlsli"

Texture2D backBuffer : register(t0);
Texture2D toMerge : register(t1);
RWTexture2D<unorm float4> output : register(u0);
sampler Sampler : register(s0);

[numthreads(16, 16, 1)]
void CS( uint3 DTid : SV_DispatchThreadID )
{
    float2 uv;
    uv.x = DTid.x / 1280.f;
    uv.y = DTid.y / 720.f;


    float3 color = backBuffer.SampleLevel(Sampler, uv, 0);
    float3 glowColor = toMerge.SampleLevel(Sampler, uv, 0);

      

    color = adjustSaturation(color, GLOW_ORIGINAL_SATURATION) * GLOW_ORIGINAL_INTENSITY;
    glowColor = adjustSaturation(glowColor, GLOW_SATURATION) * GLOW_INTENSITY;


    color *= (1 - saturate(glowColor));

    color = saturate(color + glowColor);

    output[DTid.xy] = float4(color, 1);
}