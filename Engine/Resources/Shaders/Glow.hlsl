
static const float gaussianFilter[5] = {
	0.06136,	0.24477,	0.38774,	0.24477,	0.06136
};


Texture2D inputTexture : register(t0);
RWTexture2D<float4> output;

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{

	int3 textureLocation = DTid - int3(2, 0, 0);

	float4 final = { 0.0, 0.0, 0.0, 1.0 };
	for (uint x = 0; x < 5; x++)
		final += inputTexture.Load(textureLocation + int3(x, 0, 0)) * gaussianFilter[x];
	

	output[DTid.xy] = final;
}