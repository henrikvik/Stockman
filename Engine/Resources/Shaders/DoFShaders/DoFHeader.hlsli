#pragma once
#include "../ShaderConstants.hlsli"

struct VSInput
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD;
};

SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);

#define apature .1f
#define focalLenght 1.45f
#define focalPlane 2.5f


#define CoCScale ((apature * focalLenght * (farP-nearP)) / ((focalPlane - focalLenght) * nearP * farP))
#define CoCBias ((apature * focalLenght * (nearP - focalPlane)) / ((focalPlane * focalLenght) * nearP))

float CoCBlur(Texture2D inTexture, float2 uv, float baseCoC, float4 inColor, float stepDistance, out float4 outColor)
{
    outColor = inColor;
    float4 color = inTexture.Sample(linearSampler, uv);
    bool blurNear = (color.w > 0.0f);
    //to avoid ifs
    float absCoC = abs(color.w);
    if (absCoC > stepDistance && blurNear || (baseCoC > 0.0f && absCoC < baseCoC * 2.0f))
    {
        if (blurNear)
        {
            if (outColor.w < 0.0f)
            {
                outColor.w = min(outColor.w, color.w);

            }else
            {
                if (-color.w > outColor.w)
                {
                    outColor.w = color.w;
                }

            }
        }
        float sampleFraction = saturate((absCoC - stepDistance) / ONE_OVER_SCREEN_SIZE.y);
        outColor.xyz += sampleFraction * color.xyz;

        return sampleFraction;
    }
    return 0.0f;
}