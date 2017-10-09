

Texture2D backBuffer : register(t0);
Texture2D occlusionMap : register(t1);
RWTexture2D<unorm float4> output :register(u0);

[numthreads(16, 16, 1)]
void CS( uint3 DTid : SV_DispatchThreadID )
{
    output[DTid.xy] = float4(backBuffer.Load(int3(DTid.xy, 0)).xyz * occlusionMap.Load(int3(DTid.xy, 0)).r, 1);
}