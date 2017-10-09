#include "../ShaderConstants.h"


Texture2D inputTexture : register(t0);
RWTexture2D<unorm float> output : register(u0);

[numthreads(16, 16, 1)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    int3 textureLocation = DTid - int3(0, SSAOKERNELSIZE / 2, 0);

    float final =  0.0 ;
    for (uint y = 0; y < SSAOKERNELSIZE; y++)
        final += inputTexture.Load(textureLocation + int3(0, y, 0)).r * ssaoGaussianFilter[y];
	

    output[DTid.xy] = final;
}