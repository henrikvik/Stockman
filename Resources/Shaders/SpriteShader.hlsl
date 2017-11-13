
struct Vertex
{
    float2 position;
    float2 uv;
};

struct SpriteData
{
    float yes;
};

struct Fragment
{
    float4 position : SV_Position;
    float2 uv : UV;
};

struct Pixel
{
    float4 color : SV_Target0;
};


StructuredBuffer<Vertex>     vertexData    : register(t0);
StructuredBuffer<SpriteData> spriteData    : register(t1);
Texture2D                    spriteTexture : register(t2);

SamplerState linearSampler : register(s0);

cbuffer OffsetBuffer : register(b0)
{
    uint offset;
}


Fragment VS(uint vertexId : SV_VertexId)
{
    Vertex vertex = vertexData[vertexId + offset];
    Fragment fragment = (Fragment) 0;

    fragment.position = float4(vertex.position, 0, 1);
    fragment.uv = vertex.uv;

    return fragment;
}

Pixel PS(Fragment fragment)
{
    Pixel pixel = (Pixel) 0;

    pixel.color = spriteTexture.Sample(linearSampler, fragment.uv);

    return pixel;
}