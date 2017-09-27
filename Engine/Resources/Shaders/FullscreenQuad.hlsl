struct VSIn
{
	uint vertexId : SV_VertexID;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VSOut VS(VSIn vsin)
{
    VSOut vsout;
    
	if (vsin.vertexId == 0) {
		vsout.pos = float4(-1, 3, 0, 1.0);
		vsout.uv = float2(0, -1);
	}
	else if (vsin.vertexId == 2) {
		vsout.pos = float4(-1, -1, 0, 1.0);
		vsout.uv = float2(0, 1);
	}
	else if (vsin.vertexId == 1) {
		vsout.pos = float4(3, -1, 0, 1.0);
		vsout.uv = float2(2, 1);
	}

    return vsout;
}

Texture2D albedoSpec : register(t0);
Texture2D normalMat : register(t1);
Texture2D pos : register(t2);
SamplerState sState;

float4 PS(VSOut psin) : SV_Target0
{
    //return float4(0.2, 1, 1, 1);
    return float4(albedoSpec.Sample(sState, psin.uv).xyz, 1.f);
};