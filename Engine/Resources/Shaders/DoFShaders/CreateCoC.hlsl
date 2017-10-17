#include "DoFHeader.hlsli"
struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput VS( VSInput input)
{
    VSOutput output = (VSOutput) 0;
    return output;
}

float4 PS(VSOutput input) : SV_Target0
{
    return float4(0.f, 0.f, 0.f, 0.f);

}