
Texture2D backBuffer : register(t0);
Texture2D toMerge : register(t1);
RWTexture2D<unorm float4> output : register(u0);
sampler Sampler : register(s0);

[numthreads(16, 9, 1)]
void CS( uint3 DTid : SV_DispatchThreadID )
{
    float2 uv;
    uv.x = DTid.x / 1280.f;
    uv.y = DTid.y / 720.f;


    float3 color = backBuffer.SampleLevel(Sampler, uv, 0);
    color += toMerge.SampleLevel(Sampler, uv, 0);
    color = saturate(color);

    output[DTid.xy] = float4(color, 1);

}