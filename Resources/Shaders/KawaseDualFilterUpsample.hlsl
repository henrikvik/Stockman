struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv  : UV;
};

Texture2D Input : register(t0);
SamplerState LinearClamp : register(s0);

float4 DualFilterUpsample(float2 uv, float2 offset)
{
    float4 sum = Input.Sample(LinearClamp, uv + float2(-offset.x * 2.0, 0.0));

    sum += Input.Sample(LinearClamp, uv + float2(-offset.x,       offset.y      )) * 2.0;
    sum += Input.Sample(LinearClamp, uv + float2(0.0,             offset.y * 2.0));
    sum += Input.Sample(LinearClamp, uv + float2(offset.x,        offset.y      )) * 2.0;
    sum += Input.Sample(LinearClamp, uv + float2(offset.x * 2.0,  0.0           ));
    sum += Input.Sample(LinearClamp, uv + float2(offset.x,       -offset.y      )) * 2.0;
    sum += Input.Sample(LinearClamp, uv + float2(0.0,            -offset.y * 2.0));
    sum += Input.Sample(LinearClamp, uv + float2(-offset.x,      -offset.y      )) * 2.0;

    return sum / 12.0;
}

struct PSOutput {
    float4 Output : SV_Target0;
};

PSOutput PS(VSOutput input)
{
    PSOutput output;

    output.Output = DualFilterUpsample(input.uv, float2(1, 1) / float2(1280, 720));

    return output;
}