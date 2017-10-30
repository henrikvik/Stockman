struct VSIn
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD;
    uint elm : ELEMENT;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    uint elm : ELEMENT;
};

cbuffer hudInfo : register(b0)
{
    float hpProcent;
    float offset0;
    float offset1;
    float offset2;
    float offset3;
}

cbuffer positionOffset : register(b1)
{
    float2 offset;
}

VSOut VS(VSIn vsin)
{
    VSOut vsout;

    vsout.pos = float4(vsin.pos, 0, 1);
    if (vsin.elm == 1)
    {
        if (vsin.pos.x > -.9f)
        {
            vsin.pos.x -= (abs(-1.0f - vsin.pos.x) * hpProcent);
        }
        vsout.pos = float4(vsin.pos, 0, 1);
    }else
    {
        vsout.pos = float4(vsin.pos, 0, 1);
    }

    if (vsin.elm == 3)
    {
        vsout.uv = vsin.uv;
        vsout.uv.y += offset0;
    }
    else if (vsin.elm == 4)
    {
        vsout.uv = vsin.uv;
        vsout.uv.y += offset1;
    }
    else if (vsin.elm == 5)
    {
        vsout.uv = vsin.uv;
        vsout.uv.y += offset2;
    }
    else if (vsin.elm == 6)
    {
        vsout.uv = vsin.uv;
        vsout.uv.y += offset3;
    }else
    {
        vsout.uv = vsin.uv;
    }
    

    vsout.elm = vsin.elm;


    vsout.pos.xy += offset.xy * min(vsout.elm, 1);



    return vsout;
}

Texture2D crosshair : register(t0);
Texture2D HP : register(t1);
Texture2D outline : register(t2);
Texture2D cd : register(t3);
Texture2D cdActive : register(t4);
SamplerState sState;

float4 PS(VSOut psin) : SV_Target0
{
    if (psin.elm == 0)
    {
        //return float4(1.0f, 0.0f, 0.0f, 0.0f);
        return float4(crosshair.Sample(sState, psin.uv));

    }
    else if (psin.elm == 1)
    {
        //return float4(0.0f, 1.0f, 0.0f, 1.0f);
        return float4(HP.Sample(sState, psin.uv));
    }
    else if (psin.elm == 2)
    {
        return float4(outline.Sample(sState, psin.uv));
    }else
    {
        //return float4(cdActive.Sample(sState, psin.uv));
        return float4(0.0f, 1.0f, 0.0f, 1.0f);
    }

    //return float4(HP.Sample(sState, psin.uv));
};