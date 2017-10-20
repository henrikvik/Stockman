cbuffer Camera : register(b0)
{
	float4x4 PV;
};

cbuffer matrixData
{
	float4x4 world;
};

cbuffer Time : register(b1)
{
	uint time;
};

struct Vertex
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float2 biTangent : BITANGENT;
	float2 tangent : TANGENT;
};

struct Pixel
{
	float4 ndc : SV_Position;
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv: UV;
};

Pixel VS(Vertex vertex)
{
	Pixel pixel;
	float3 position = vertex.position;

	float sx = sin(position.x + vertex.uv.x * 5 + time * 0.018);
	float sy = cos(position.y + pow(vertex.uv.y, 2) * 5 + time * 0.018);

	position.x += 0.1 * sx *sy* position.y * 2;
	position.z += 0.05 * sy*sx*sy * position.y * 2;
	pixel.uv = vertex.uv;

	pixel.ndc = mul(PV, float4(position, 1));

	return pixel;
}

Texture2D Image : register(t0);
SamplerState sState;

float4 PS(Pixel pixel) : SV_Target
{
	float4 color = Image.Sample(sState, pixel.uv).rgba;

	/*float3 r = color.rrr;
	float3 g = color.ggg;
	float3 b = color.bbb;
	*/

	float3 finalColor = color;

	float alpha = color.a;

	clip(alpha - 0.5);

	return float4(finalColor, alpha);
}