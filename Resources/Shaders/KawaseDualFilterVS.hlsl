struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv  : UV;
};

VSOutput VS(uint id: SV_VertexID)
{
    VSOutput output;

    output.uv = float2((id << 1) & 2, id & 2);
    output.pos = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);

    return output;
}