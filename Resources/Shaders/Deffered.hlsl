// VERTEX SHADER

cbuffer Camera : register(b0)
{
    float4x4 VP;
};

struct VSInput
{
    // Vertex data
    float3 pos : POS;
    float2 uv : UV;
    float3 normal : NORMAL;
    uint material : MATERIAL;

    float3 offset : OFFSET;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 wPos : WPOS;
    float2 uv : UV;
    float3 normal : NORMAL;
    nointerpolation uint material : MATERIAL;
};

VSOutput VS(VSInput input, uint id : SV_InstanceID)
{
    VSOutput output;

    output.pos = float4(input.pos, 1);
    output.pos.xyz += input.offset.xyz;
    output.pos = mul(VP, output.pos);
    output.uv = input.uv;
    output.normal = input.normal;

    output.wPos = float4(input.pos, 1);
    output.material = 0;

    return output;
}


// PIXEL SHADER

struct PSOutput
{
    float4 albedoSpecular : SV_Target0;
    float4 normalMat : SV_Target1;
    float4 position : SV_Target2;
};

Texture2D textureeee : register(t0);
SamplerState sState;

PSOutput PS(VSOutput input)
{
    PSOutput output;

    output.normalMat.xyz = input.normal.xyz;
    output.normalMat.w = input.material;
    output.albedoSpecular.xyz = textureeee.Sample(sState, input.uv).xyz;
    output.albedoSpecular.w = 10;
    output.position = input.wPos;

    return output;
}