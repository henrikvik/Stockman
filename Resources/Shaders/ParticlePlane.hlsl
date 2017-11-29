

struct Vertex
{
    float4 position : SV_Position;
    float2 uv : UV;
};

struct Pixel
{
    float4 color : SV_Target0;
};

StructuredBuffer<Vertex> vertexData    : register(t0);
StructuredBuffer<ParticleData> particleData    : register(t1);
Texture2D ParticleTexture : register(t2);

SamplerState linearSampler : register(s0);

Fragment VS(uint vertexId : SV_VertexId)
{
    Vertex vertex = (Vertex)0;

    vertex.position = float4(vertex.position, 0, 1);
    vertex.uv = vertex.uv;

    return fragment;
}

Pixel PS(Fragment fragment)
{
    Pixel pixel = (Pixel)0;

    pixel.color = spriteTexture.Sample(linearSampler, fragment.uv);

    pixel.color = float4(pixel.color.xyz, min(pixel.color.w, spriteData[fragment.spriteIndex].aplha));
    return pixel;
}