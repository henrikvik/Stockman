#include "DoFHeader.hlsli"
struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput VS(uint id: SV_VertexID)
{
    VSOutput vsout;
    
	vsout.uv = float2((id << 1) & 2, id & 2);
	vsout.pos = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);


    return vsout;
}

Texture2D blurT0 : register(t0);
Texture2D blurT1 : register(t1);

float4 PS(VSOutput input) : SV_Target0
{
    half4 finalColor;

    half4 baseColor = blurT1.Sample(linearSampler, input.uv);


    half4 color0 = half4(0.0f, 0.0f, 0.0f, baseColor.w);
    half4 color1 = half4(0.0f, 0.0f, 0.0f, baseColor.w);
    half sampelCount0 = 0.0f;
    half sampleCount1 = 0.0f;

    half diagonalStep = 0.866f * (ONE_OVER_SCREEN_SIZE.x / ONE_OVER_SCREEN_SIZE.y);
    half step0 = float2(diagonalStep, -0.5f);
    half step1 = float2(-diagonalStep, -0.5f);

    for (int i = 0; i < DoFkernel; i++)
    {
        half stepDistance = (i + 0.5f) * ONE_OVER_SCREEN_SIZE.y;

        sampelCount0 += CoCBlur(blurT0, (input.uv + (step0 * stepDistance)), baseColor.w, color0, stepDistance, color0);
        sampleCount1 += CoCBlur(blurT1, input.uv + (step1 * stepDistance), baseColor.w, color1, stepDistance, color1);

    }

    color0.xyz = sampelCount0 > 0 ? color0.xyz / sampelCount0 : baseColor.xyz * .5f;
    color1.xyz = sampleCount1 > 0 ? color1.xyz / sampleCount1 : baseColor.xyz;

    finalColor.xyz = (color0.xyz + color1.xyz) / 3.0f;

    finalColor.w = max(abs(color0.w), abs(color1.w));

    return finalColor;

}