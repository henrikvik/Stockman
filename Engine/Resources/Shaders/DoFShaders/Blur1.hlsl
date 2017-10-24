#include "DoFHeader.hlsli"
struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};
struct PSOutput
{
    half4 color0 : SV_Target0;
    half4 color1 : SV_Target1;
};

VSOutput VS(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.uv = input.uv;
    return output;
}


Texture2D CoCTexture : register(t0);

PSOutput PS(VSOutput input) : SV_Target0
{
    PSOutput output = (PSOutput) 0.0f;
    float4 baseColor = CoCTexture.Sample(linearSampler, input.uv);
    
    output.color0.w = baseColor.w;
    output.color1.w = baseColor.w;

    float sampelCount0 = 0.0f;
    float sampelCount1 = 0.0f;

    float diagonalStep = 0.866f * (ONE_OVER_SCREEN_SIZE.x / ONE_OVER_SCREEN_SIZE.y);
    for (int i = 0; i < DoFkernel; i++)
    {
        float stepDistance = (i + 0.5f) * ONE_OVER_SCREEN_SIZE.y;
        

        //vetical
        float2 step0 = float2(0.0f, 1.0f) * stepDistance;
        sampelCount0  += CoCBlur(CoCTexture, input.uv + step0, baseColor.w, output.color0, stepDistance, output.color0);

        //diagonal
        float step1 = float2(diagonalStep, 1.0f) * stepDistance;
        sampelCount1 += CoCBlur(CoCTexture, input.uv + step1, baseColor.w, output.color1, stepDistance, output.color1);

    }

    output.color0.xyz = sampelCount0 > 0 ? (output.color0.xyz / sampelCount0) : baseColor.xyz;
    output.color1.xyz = sampelCount1 > 0 ? (output.color1.xyz / sampelCount1) : baseColor.xyz;

    output.color1.xyz += output.color0.xyz;

    if (abs(output.color0.w) > abs(output.color1.w))
    {
        output.color1.w = output.color0.w;
    }

    return output;
}