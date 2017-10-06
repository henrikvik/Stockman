#pragma once
#define KERNELSIZE 7
#define SSAOKERNELSIZE 7
static const float gaussianFilter[KERNELSIZE] =
{
	0.070159, 0.131075, 0.190713, 0.216106, 0.190713, 0.131075, 0.070159
};


static const float ssaoGaussianFilter[SSAOKERNELSIZE] =
{
	0.070159, 0.131075, 0.190713, 0.216106, 0.190713, 0.131075, 0.070159
};