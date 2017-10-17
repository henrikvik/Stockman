#include "LightCalcInclude.hlsli"

cbuffer matrixData : register(b3)
{
	float4x4 world;
};

cbuffer Time : register(b4)
{
	uint grassTime;
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
    float3 lightPos : LIGHT_POSITION;
	float3 normal : NORMAL;
	float2 uv: UV;
};

Pixel VS(Vertex vertex)
{
	Pixel pixel;
	float3 position = vertex.position;
    pixel.normal = vertex.normal;
    pixel.uv = vertex.uv;

    float sx = sin(position.x + vertex.uv.x * 5 + grassTime * 0.018);
    float sy = cos(position.y + pow(vertex.uv.y, 2) * 5 + grassTime * 0.018);

	position.x += 0.1 * sx *sy* position.y * 2;
	position.z += 0.05 * sy*sx*sy * position.y * 2;
    pixel.lightPos = mul(lightVP, float4(position, 1));
    pixel.position = position;

    pixel.ndc = mul(ViewProjection, float4(position, 1));

	return pixel;
}

float4 PS(Pixel pixel) : SV_Target
{
    float shadow = calculateShadowValue(pixel.lightPos, 2);
    
    float4 color = calculateDiffuseLight(pixel.position, pixel.lightPos, pixel.ndc.xyz, pixel.uv, pixel.normal, shadow);

	clip(color.a - 0.5);

    //return float4(pixel.normal, 1);
    return color;
}