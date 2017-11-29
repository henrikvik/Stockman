#include "LightCalcInclude.hlsli"

#define SNOW_RADIUS 50.f
#define FADE_DIST 30.f
#define SNOW_COLOR float3(0.7f, 0.7f, 0.7f)
struct Vertex
{
    float3 pos : POSITION;
    float rot  : ROTATIONVALUE;
    float distance : DISTANCE;
};

StructuredBuffer<Vertex> vertexBuffer : register(t4);

Vertex VS(uint vertexId : SV_VertexId)
{
    return vertexBuffer[vertexId];
}

struct GS_OUT
{
    float4 pos : SV_Position;
    float4 wPos : WORLDPOS;
    float4 lPos : LIGHTPOS;
    float distance : DISTANCE;
};

[maxvertexcount(3)]
void GS(point Vertex input[1], inout TriangleStream<GS_OUT> output)
{
    GS_OUT snow = (GS_OUT)0;
    snow.distance = input[0].distance;

    float rot = input[0].rot;
    float4x4 rotation = 
    {
        cos(rot), -sin(rot),  -sin(rot),      0,

        sin(rot),  cos(rot),   cos(rot),      0,

        sin(rot),  sin(rot),   cos(rot),      0,

            0,         0,          0,         1
    };

    float4 positions[3] = 
    { 
        -0.5, -0.5, 0, 0,
         0.5, -0.5, 0, 0,
           0,  0.5, 0, 0
    };

    for (int i = 0; i < 3; i++)
    {
        positions[i] *= max((snow.distance / SNOW_RADIUS), 0.2f);
        positions[i] = mul(rotation, positions[i]);
    }

    float4 temp = float4(input[0].pos, 1);
    snow.wPos = temp.xyzw + positions[0];
    snow.lPos = mul(lightVP, snow.wPos);
    snow.pos = mul(ViewProjection, snow.wPos);
    output.Append(snow);

    snow.wPos = temp + positions[1];
    snow.lPos = mul(lightVP, snow.wPos);
    snow.pos = mul(ViewProjection, snow.wPos);
    output.Append(snow);

    snow.wPos = temp + positions[2];
    snow.lPos = mul(lightVP, snow.wPos);
    snow.pos = mul(ViewProjection, snow.wPos);
    output.Append(snow);
}


float4 PS(GS_OUT input) : SV_TARGET
{

    float shadow = max(calculateShadowValue(input.lPos.xyz, 2), 0.2f);

    uint2 tile = uint2(floor(input.pos.xy / BLOCK_SIZE));
    uint offset = LightGrid[tile].x;
    uint count = LightGrid[tile].y;

    float3 lighting = SNOW_COLOR * shadow;

    for (uint i = 0; i < count; i++)
    {
        uint idx = LightIndexList[offset + i];
        Light light = Lights[idx];

        float3 posToLight = light.positionWS - input.wPos.xyz;

        float distance = length(posToLight);
        float3 normalizedLight = posToLight / distance;
        float attenuation = 1.0f - smoothstep(0, light.range, distance);

        lighting += light.color * attenuation * light.intensity;
    }

    lighting = saturate(lighting);

    return float4(lighting, (SNOW_RADIUS - input.distance) / FADE_DIST);
}