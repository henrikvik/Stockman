#pragma once
#define SCREEN_SIZE float2(1280, 720)


#define KERNELSIZE 7
static const float gaussianFilter[KERNELSIZE] =
{
	0.070159, 0.131075, 0.190713, 0.216106, 0.190713, 0.131075, 0.070159
};



#pragma region SSAO constants
#define SSAOKERNELSIZE 5
static const float ssaoGaussianFilter[SSAOKERNELSIZE] =
{
	0.192051, 0.203926, 0.208046, 0.203926, 0.192051
};

#define RANDOM_SIZE 256
#define SAMPLE_RADIUS 0.5f
#define INTENSITY 5.0
#define SCALE 1.f
#define BIAS 0.3f
#define ITERATIONS 4

#pragma endregion


//makes stuff gray
float3 adjustSaturation(float3 color, float saturation)
{
	float grey = dot(color, float3(0.3, 0.59, 0.11));

	return lerp(grey, color, saturation);
}

float3 adjustContrast(float3 color, float contrast, float threshold)
{
    return (color - threshold) * max(contrast, 0.f) + threshold;
}