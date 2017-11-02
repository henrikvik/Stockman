#pragma once
#define SCREEN_SIZE float2(1280, 720)
#define ONE_OVER_SCREEN_SIZE float2(1.0f/1280.0f, 1.0f/720.0f)
#define nearP 0.1f
#define farP 250.f

///GLOW CONSTANTS///
#define MIP_LEVELS 5
#define KERNELSIZE 5
static const float gaussianFilter[KERNELSIZE] =
{
	0.192051, 0.203926, 0.208046, 0.203926, 0.192051 
};

#define GLOW_INTENSITY 1.f

//Darkens ENTIRE SCENE exept for glowing area
#define GLOW_ORIGINAL_INTENSITY 1.f

#define GLOW_SATURATION 1.f
#define GLOW_ORIGINAL_SATURATION 1.f





///SSAO CONSTANTS////
#define SSAOKERNELSIZE 5
static const float ssaoGaussianFilter[SSAOKERNELSIZE] =
{
	0.192051, 0.203926, 0.208046, 0.203926, 0.192051
};

#define SSAO_RANDOM_SIZE 256
#define SSAO_SAMPLE_RADIUS 0.5f
#define SSAO_INTENSITY 5.0
#define SSAO_SCALE 1.f
#define SSAO_BIAS 0.3f
#define SSAO_ITERATIONS 4
