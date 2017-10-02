/*struct VSInput
{
	// Vertex data
	float3 pos : POS;
	float2 uv : UV;
	float3 normal : NORMAL;
	uint material : MATERIAL;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 wPos : WPOS;
	float2 uv : UV;
	float3 normal : NORMAL;
};*/


//Vertex Shader
VSOutput VS(VSInput input, uint id : SV_InstanceID)
{
	VSOutput output;

	return output;
}



//Pixel Shader
/*struct PSOutput
{
	float4 position : SV_Target2;
};*/

SamplerState sState;

PSOutput PS(VSOutput input)
{
	PSOutput output;

	return output;
}