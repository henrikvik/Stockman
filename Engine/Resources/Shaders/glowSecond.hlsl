
static const float gaussianFilter[5] = {
	0.06136,	0.24477,	0.38774,	0.24477,	0.06136
};


Texture2D inputTexture : register(t0);
RWTexture2D<float4> output;

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

	int3 textureLocation = DTid - int3(0, 2, 0);

	float4 final = { 0.0, 0.0, 0.0, 1.0 };
	for (uint y = 0; y < 5; y++)
		final += inputTexture.Load(textureLocation + int3(0, y, 0)) * gaussianFilter[y];
	

	output[DTid.xy] = final;
}