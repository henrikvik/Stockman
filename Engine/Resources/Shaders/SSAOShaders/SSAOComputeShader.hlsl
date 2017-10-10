#include "../ShaderConstants.h"


Texture2D depthTexture : register(t0);
Texture2D randomNormalTexture : register(t1);
Texture2D normalTexture : register(t2);
RWTexture2D<unorm float> output: register(u0);
SamplerState Sampler : register(s0);
SamplerState RandomSampler : register(s1);


cbuffer Camera : register(b0)
{
    float4x4 InvView;
    float4x4 InvProjection;
}

//probably broken
float getLinearDepth(float near, float far, float2 uv)
{
    return 2.0 * near / (far + near - depthTexture.SampleLevel(Sampler, uv, 0).r * (far - near));
}

//Not yet implemeneted
float3 generateNormal(float depth, float2 uv, float2 screenSize)
{
    return float3(0, 0, 0);
}

float3 generateViewSpacePos(float2 uv, float2 offset = float2(0, 0))
{
    float depth = depthTexture.SampleLevel(Sampler, uv + offset, 0).r;

    float4 pos;
    pos.x = (uv.x + offset.x) * 2 - 1;
    pos.y = (1 - uv.y + offset.y) * 2 - 1;
    pos.z = depth;
    pos.w = 1;

    pos = mul(InvProjection, pos);

    pos /= pos.w;

    return pos.xyz;
}

float caculateOcclusion(float2 texCoord, float2 uv, float3 pos, float3 normal)
{
    float3 difference = generateViewSpacePos(texCoord + uv) - pos;

    float3 vec = normalize(difference);
    float vecLength = length(difference) * SCALE;
    float nDotVec = dot(normal, vec);
    float occlusion = max(0.0, nDotVec - BIAS) * (1.0 / (1.0 + vecLength)) * INTENSITY;
    return occlusion;
}


[numthreads(16, 16, 1)]
void CS( uint3 DTid : SV_DispatchThreadID )
{
    float2 uv = float2(DTid.xy * 2.0 / SCREEN_SIZE);

    float occlusion = 0;

    const float2 vectors[4] =
    {
        float2(1, 0), float2(-1, 0),
        float2(0, 1), float2(0, -1)
    };

    //Remove magic 256 and make a define
    float2 randomNormal = normalize(randomNormalTexture.SampleLevel(RandomSampler, (uv * SCREEN_SIZE / RANDOM_SIZE), 0).xy * 2 - 1);

    float3 normal = normalTexture.SampleLevel(Sampler, uv, 0).xyz;
    
    float3 pos = generateViewSpacePos(uv);

    float depthRadius = SAMPLE_RADIUS / pos.z;

    float magicValue = 0.707;

    for (int i = 0; i < ITERATIONS; i++)
    {
        float2 coord0 = reflect(vectors[i], randomNormal) * depthRadius;
        float2 coord1 = float2(coord0.x * magicValue - coord0.y * magicValue,
        coord0.x * magicValue + coord0.y * magicValue);

        occlusion += caculateOcclusion(uv, coord0 * 0.25, pos, normal.xyz);
        occlusion += caculateOcclusion(uv, coord1 * 0.5,  pos, normal.xyz);
        occlusion += caculateOcclusion(uv, coord0 * 0.75, pos, normal.xyz);
        occlusion += caculateOcclusion(uv, coord1,        pos, normal.xyz);
    }
    occlusion /= float(ITERATIONS) * 4.0;
    
    output[DTid.xy] = saturate(1 - occlusion);
}