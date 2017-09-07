struct VSIn
{
    float2 pos : POSITION;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VSOut VS(VSIn vsin)
{
    VSOut vsout;

    vsout.pos = float4(vsin.pos, 0, 1);
    
    vsout.uv = vsin.pos;
    vsout.uv = vsout.uv * 0.5 + 0.5;
    vsout.uv.y = 1 - vsout.uv.y;
    return vsout;
}

Texture2D textureeee : register(t0);
SamplerState sState;

float4 PS(VSOut psin) : SV_Target0
{
    //return float4(0.2, 1, 1, 1);
    return float4(textureeee.Sample(sState, psin.uv).xyz, 1);
};