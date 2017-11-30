struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv  : UV;
};

Texture2D Input : register(t0);
SamplerState LinearClamp : register(s0);

float4 DualFilterDownsample(float2 uv, float2 offset)
{
    float4 sum = Input.Sample(LinearClamp, uv) * 4.0;

    sum += Input.Sample(LinearClamp, uv - offset);
    sum += Input.Sample(LinearClamp, uv + offset);
    sum += Input.Sample(LinearClamp, uv + float2(1, -1) * offset);
    sum += Input.Sample(LinearClamp, uv - float2(1, -1) * offset);

    return sum / 8.0;
}

struct PSOutput {
    float4 Output : SV_Target0;
};

PSOutput PS(VSOutput input)
{
    PSOutput output;

    output.Output = DualFilterDownsample(input.uv, float2(1, 1) / float2(1280, 720));

    return output;
}