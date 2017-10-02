struct VS_Input
{
	float3 pos : POSITION;
	float3 color : COLOR;
};

struct VS_Output
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

VS_Output main(VS_Input input)
{
	VS_Output output;

	output.position = float4(input.position.x, input.position.y, input.position.z, 1);

	output.color = input.color;

	return output;
}