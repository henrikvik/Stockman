#include "DoFHeader.hlsli"
struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput VS(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.uv = input.uv;
    return output;
}

Texture2D blurT0 : register(t0);
Texture2D blurT1 : register(t1);

float4 PS(VSOutput input) : SV_Target0
{
    float4 finalColor;

    float4 baseColor = blurT1.Sample(linearSampler, input.uv);


    float4 color0 = float4(0.0f, 0.0f, 0.0f, baseColor.w);
    float4 color1 = float4(0.0f, 0.0f, 0.0f, baseColor.w);
    float sampelCount0 = 0.0f;
    float sampleCount1 = 0.0f;

    float diagonalStep = 0.866f * (ONE_OVER_SCREEN_SIZE.x / ONE_OVER_SCREEN_SIZE.y);
    float step0 = float2(diagonalStep, -0.5f);
    float step1 = float2(-diagonalStep, -0.5f);

    for (int i = 0; i < 8; i++)
    {
        float stepDistance = (i + 0.5f) * ONE_OVER_SCREEN_SIZE.y;

        sampelCount0 += CoCBlur(blurT0, (input.uv + (step0 * stepDistance)), baseColor.w, color0, stepDistance, color0);
        sampleCount1 += CoCBlur(blurT1, input.uv + (step1 * stepDistance), baseColor.w, color1, stepDistance, color1);

    }

    color0.xyz = sampelCount0 > 0 ? color0.xyz / sampelCount0 : baseColor.xyz * .5f;
    color1.xyz = sampleCount1 > 0 ? color1.xyz / sampleCount1 : baseColor.xyz;

    finalColor.xyz = (color0.xyz + color1.xyz) / 3.0f;

    finalColor.w = max(abs(color0.w), abs(color1.w));

    return finalColor;

}