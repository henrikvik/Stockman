struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VSOut VS(uint id: SV_VertexID)
{
    VSOut vsout;
    
	vsout.uv = float2((id << 1) & 2, id & 2);
	vsout.pos = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);

    return vsout;
}

Texture2D tex : register(t0);
SamplerState sState;

float4 PS(VSOut psin) : SV_Target0
{
    return float4(tex.Sample(sState, psin.uv).xyz, 1.f);
};