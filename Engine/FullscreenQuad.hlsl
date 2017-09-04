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
    vsout.uv = vsin.pos * 0.5 + 0.5;

    return vsout;
}

Texture2D texture : register(t0);

float4 PS(VSOut psin) : SV_Target0
{
    return float4(texture.Load(int3(psin.uv, 0)).xyz, 1);
};