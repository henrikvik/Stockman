// VERTEX SHADER

cbuffer Camera : register(b0) {
    float4x4 WVP;
};

struct VSInput {
    // Vertex data
    float3 pos : POS;
    float2 uv : UV;
    float3 normal : NORMAL;
    uint material : MATERIAL;

    // Instancing data
    float4x4 translation : TRANSLATION;
};

struct VSOutput {
    float4 pos : SV_POSITION;
    nointerpolation uint material : MATERIAL;
};

VSOutput VS(VSInput input, uint id : SV_InstanceID)
{
    VSOutput output;

    output.pos = float4(input.pos, 1);

	return output;
}


// PIXEL SHADER

struct PSOutput {
    float4 albedoSpecular : SV_Target0;
    float4 normalMat : SV_Target1;
    float4 position : SV_Target2;
};

PSOutput PS(VSOutput input)
{
    PSOutput output;

    output.normalMat.w = input.material;
    output.albedoSpecular = float4(1, 0, 0, 0);

    return output;
}