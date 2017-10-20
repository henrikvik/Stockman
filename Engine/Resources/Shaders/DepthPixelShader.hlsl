struct Pixel
{
    float4 ndc : SV_Position;
    float3 position : POSITION;
    float3 lightPos : LIGHT_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

SamplerState Sampler : register(s0);
Texture2D diffuseMap : register(t10);

void PS(Pixel pixel)
{
    float4 color = diffuseMap.Sample(Sampler, pixel.uv);
    clip(color.a - 0.5);
}