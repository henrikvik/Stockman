struct VSIn
{
    float2 pos : POSITION;
    uint elm : ELEMENT;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    uint elm : ELEMENT;
};

VSOut VS(VSIn vsin)
{
    VSOut vsout;

    vsout.pos = float4(vsin.pos, 0, 1);
    
    vsout.uv = vsin.pos;
    vsout.uv = vsout.uv * 0.5 + 0.5;
    vsout.uv.y = 1 - vsout.uv.y;

    vsout.elm = vsin.elm;
    return vsout;
}

Texture2D crosshair : register(t0);
Texture2D HP : register(t1);
SamplerState sState;

float4 PS(VSOut psin) : SV_Target0
{
    if(psin.elm == 0)
    {
        return float4(1.0f, 0.0f, 0.0f, 0.0f);
        //return float4(crosshair.Sample(sState, psin.uv));

    }else
    {
        return float4(0.0f, 1.0f, 0.0f, 0.0f);
        //return float4(HP.Sample(sState, psin.uv));
    }
};