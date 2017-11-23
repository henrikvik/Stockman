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

VSOutput VS(uint id: SV_VertexID)
{
    VSOutput vsout;
    
	vsout.uv = float2((id << 1) & 2, id & 2);
	vsout.pos = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);


    return vsout;
}


Texture2D CoCTexture : register(t0);

PSOutput PS(VSOutput input) : SV_Target0
{
    PSOutput output = (PSOutput) 0.0f;
    half4 baseColor = CoCTexture.Sample(linearSampler, input.uv);
    
    output.color0.w = baseColor.w;
    output.color1.w = baseColor.w;

    half sampelCount0 = 0.0f;
    half sampelCount1 = 0.0f;

    half diagonalStep = 0.866f * (ONE_OVER_SCREEN_SIZE.x / ONE_OVER_SCREEN_SIZE.y);
    for (int i = 0; i < DoFkernel; i++)
    {
        half stepDistance = (i + 0.5f) * ONE_OVER_SCREEN_SIZE.y;
        

        //vetical
        half2 step0 = half2(0.0f, 1.0f) * stepDistance;
        sampelCount0  += CoCBlur(CoCTexture, input.uv + step0, baseColor.w, output.color0, stepDistance, output.color0);

        //diagonal
        half step1 = half2(diagonalStep, 1.0f) * stepDistance;
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