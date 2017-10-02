struct PS_Input
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

float4 main(PS_Input input) : SV_TARGET
{
	return float4(input.color.r, input.color.g, input.color.b, 1);
}