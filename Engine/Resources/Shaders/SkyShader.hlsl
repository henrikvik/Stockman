#include "include/Vertex.hlsli"

#include "LightCalcInclude.hlsli"

struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 lPos : WORLD_POSITION;
    float2 uv : UV;
};

cbuffer transform : register(b4)
{
    float4x4 transform;
};

cbuffer SkyColors : register(b5)
{
    float4 Base;
    float4 Top;
    float4 AngleSize;
}

PS_IN VS(Vertex vertex)
{
    PS_IN output = (PS_IN) 0;

    output.pos = float4(vertex.position, 1);

    output.pos = mul(transform, output.pos);
    output.pos = mul(ViewProjection, output.pos).xyww;
    output.uv = vertex.uv;

    output.lPos = vertex.position;

    return output;
}

float4 PS(PS_IN input) : SV_Target0
{
    float3 nightGradient = lerp(Base.xyz, Top.xyz, input.lPos.y);
    
    float3 sphereDir = normalize(input.lPos);
    float angle = saturate(dot(sphereDir, AngleSize.xyz));
    angle = pow(angle, 16);
    float3 moonColor = float3(0.4f, 0.35f, 0.25f);

    float3 col = saturate(nightGradient + float3(moonColor) * angle);

    return float4(col, 1);
}