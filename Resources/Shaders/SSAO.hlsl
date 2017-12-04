struct VSOutput
{
    float4 position : SV_Position;
    float2 uv       : UV;
};

VSOutput VS(uint id: SV_VertexID)
{
    VSOutput vsout;

    vsout.uv = float2((id << 1) & 2, id & 2);
    vsout.position = float4(vsout.uv * float2(2, -2) + float2(-1, 1), 0, 1);

    return vsout;
}

cbuffer SSAOConstants : register(b0)
{
    float4x4 Projection;
    float4x4 View;
    float4 Kernel[32];
    float2 NoiseScale;
}

Texture2D Position : register(t0);
Texture2D Normal : register(t1);
Texture2D Random : register(t2);

SamplerState PointWrap : register(s0);

float PS(VSOutput input) : SV_Target0
{
    float3 position = Position.Sample(PointWrap, input.uv).xyz;
    float3 normal = Normal.Sample(PointWrap, input.uv).xyz * 2 - 1;
    normal = normalize(normal);

    float3 random = float3(Random.Sample(PointWrap, input.uv * float2(1280 / 4.0,720 / 4.0)).xy, 0);


    float3 tangent = normalize(random - normal * dot(random, normal));
    float3 bitangent = cross(normal, tangent);
    float3x3 tbn = float3x3(
        tangent.x, bitangent.x, normal.x,
        tangent.y, bitangent.y, normal.y,
        tangent.z, bitangent.z, normal.z
        );
    float radius = 0.851395f;
    float bias = 0.025f;

    float occlusion = 0.f;
    for (int i = 0; i < 16; i++) {
        float3 s = mul(tbn, Kernel[i].xyz);
        s = position + s * radius;

        float4 offset = float4(s, 1.f);
        offset = mul(Projection, offset);
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5f + 0.5f;
        offset.y = 1 - offset.y;

        float depth = Position.Sample(PointWrap, saturate(offset.xy)).z;

        float rangeCheck = abs(position.z - depth) < radius ? 1.0 : 0.0;//smoothstep(0.0, 1.0, radius / abs(position.z - depth));
        occlusion += (depth >= s.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    return 1.0 - (occlusion * 4 / 8.0);
}