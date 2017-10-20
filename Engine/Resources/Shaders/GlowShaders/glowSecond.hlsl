#include "../ShaderConstants.hlsli"


Texture2D inputTexture : register(t0);
RWTexture2D<unorm float4> output : register(u0);

[numthreads(16, 9, 1)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    int3 textureLocation = DTid - int3(0, KERNELSIZE / 2, 0);

	float4 final = { 0.0, 0.0, 0.0, 1.0 };
    for (uint y = 0; y < KERNELSIZE; y++)
		final += inputTexture.Load(textureLocation + int3(0, y, 0)) * gaussianFilter[y];
	

	output[DTid.xy] = final;
}