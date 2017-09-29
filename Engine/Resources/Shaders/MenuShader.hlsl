struct VSIn
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOut VS(VSIn vsin)
{
    VSOut vsout;

    vsout.pos = float4(vsin.pos, 1.0f);
    vsout.uv = vsin.uv;
 
    return vsout;
}

Texture2D image : register(t0);
SamplerState sState;

float4 PS(VSOut psin) : SV_Target0
{
    return float4(image.Sample(sState, psin.uv));
};