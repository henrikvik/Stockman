#include "ShaderConstants.hlsli"


Texture2D inputTexture : register(t0);
SamplerState Sampler : register(s0);

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

float4 PS(VSOut input) : SV_Target0
{
    float3 color = float3(0, 0, 0);

    color += 0.25 * inputTexture.SampleLevel(Sampler, input.uv, 0).rgb;
    color += 0.25 * inputTexture.SampleLevel(Sampler, input.uv, 1).rgb;
    color += 0.5 * inputTexture.SampleLevel(Sampler, input.uv, 2).rgb;

    /*for (int i = 0; i < 3; i++)
    {
    color += inputTexture.SampleLevel(Sampler, input.uv, i).rgb;
    color *= color;
    }*/
    //color = saturate(color);

    return float4(color, 1);
}