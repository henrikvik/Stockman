[numthreads(16, 16, 1)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    const float strenght = 1.3;
    const float base = 0.2;
    const float area = 0.0075;
    const float falloff = 0.000001;
    const float radius = 0.0002;
    const int samples = 16;


    const float2 screenSize = float2(1280, 720);

    float2 uv = float2(DTid.xy / screenSize);

    float occlusion = 0;

    const float2 vectors[4] =
    {
        float2(1, 0), float2(-1, 0),
        float2(0, 1), float2(0, -1)
    };

    float3 sampleSphere[samples] =
    {
        float3(0.5381, 0.1856, -0.4319), float3(0.1379, 0.2486, 0.4430),
      float3(0.3371, 0.5679, -0.0057), float3(-0.6999, -0.0451, -0.0019),
      float3(0.0689, -0.1598, -0.8547), float3(0.0560, 0.0069, -0.1843),
      float3(-0.0146, 0.1402, 0.0762), float3(0.0100, -0.1924, -0.0344),
      float3(-0.3577, -0.5301, -0.4358), float3(-0.3169, 0.1063, 0.0158),
      float3(0.0103, -0.5869, 0.0046), float3(-0.0897, -0.4940, 0.3287),
      float3(0.7119, -0.0154, -0.0918), float3(-0.0533, 0.0596, -0.5411),
      float3(0.0352, -0.0631, 0.5460), float3(-0.4776, 0.2847, -0.0271)
    };

    //Remove magic 256 and make a define
    float3 randomNormal = normalize(randomNormalTexture.SampleLevel(RandomSampler, (uv * screenSize / 256), 0).xyz);

    float depth = depthTexture.SampleLevel(Sampler, uv, 0).r;

    float4 normal;
    
    
    float3 pos = generateViewSpacePos(uv);
    float3 pos0 = generateViewSpacePos(uv, float2(0, 1) / screenSize);
    float3 pos1 = generateViewSpacePos(uv, float2(1, 0) / screenSize);

    float3 v0 = pos0 - pos;
    float3 v1 = pos1 - pos;

    normal.xyz = cross(v1, v0);
    normal.y = -normal.y;
    normal.xyz = normalize(normal.xyz);
    normal.w = 0;

    float radiusDepth = radius / depth;



    for (int i = 0; i < samples; i++)
    {
        float3 ray = radiusDepth * reflect(sampleSphere[i], randomNormal);
        float3 hemiRay = pos.xyz + sign(dot(ray, normal.xyz)) * ray;

        float occlusionDepth = depthTexture.SampleLevel(Sampler, saturate(hemiRay.xy), 0);
        float diff = depth - occlusionDepth;

        occlusion += step(falloff, diff) * (1.0 - smoothstep(falloff, area, diff));

    }

    float finalOcclusion = 1 - (strenght * occlusion * (1.0 / samples));
    output[DTid.xy] = saturate(finalOcclusion + base);
    //output[DTid.xy] = float4(normal.xyz, 1);

}