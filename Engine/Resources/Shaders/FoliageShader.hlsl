cbuffer Camera : register(b0)
{
	float4x4 PV;
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
	
	
	float s = sin(time * 0.01);
	float3 position = vertex.position;
	position.x += 0.1 * s * position.y * position.y * 2;

	pixel.uv = vertex.uv;

	pixel.ndc = mul(PV, float4(position, 1));

	return pixel;
}

Texture2D image : register(t0);
SamplerState sState;

float4 PS(Pixel pixel) : SV_Target
{
	return float4(image.Sample(sState, pixel.uv));;
}