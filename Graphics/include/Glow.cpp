#include "Glow.h"
#include <WICTextureLoader.h>
#include "ThrowIfFailed.h"
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#define KERNELSIZE 9
#define SIGMA 5

Graphics::Glow::Glow(ID3D11Device * device, ID3D11DeviceContext * context)
	: glow(device, SHADER_PATH("GlowShaders/Glow.hlsl"))
	, glow2(device, SHADER_PATH("GlowShaders/GlowSecond.hlsl"))
	, merger(device, SHADER_PATH("GlowShaders/Merger.hlsl"))
	, glowPass0(device, WIN_WIDTH / 2, WIN_HEIGHT / 2)
	, glowPass1(device, WIN_WIDTH / 2, WIN_HEIGHT / 2)
{
	this->states = newd DirectX::CommonStates(device);

	//Enable this only if you want a new gaussian filter
	//auto kernels = generateKernel(KERNELSIZE, SIGMA);

}

Graphics::Glow::~Glow()
{
	delete states;
}

std::vector<float> Graphics::Glow::generateKernel(int kernelSize, float sigma) 
{
	if (kernelSize % 2 == 0) kernelSize--;
	
	std::vector<float> kernel(kernelSize);

	auto g = [&](int x) -> float {
		static float gMult = 1.0f / (sqrt(2.0f * M_PI) * sigma);
		return gMult * pow(M_E, -(pow(x, 2) / (2.0f * pow(sigma, 2))));
	};

	float kernelSum = 0;
	int halfWidth = (kernelSize + 1) / 2;
	for (int x = 0; x < halfWidth; x++) {
		float factor = g(x);
		kernelSum += factor * (x == 0 ? 1 : 2);
		kernel[halfWidth-1 + x] = factor;
		kernel[halfWidth-1 - x] = factor;
	}

	for (auto& k : kernel) {
		k /= kernelSum;
	}

	std::wstring output = L"";

	for (size_t i = 0; i < kernel.size(); i++)
	{
		output += std::to_wstring(kernel[i]);
		output += L", ";
	}
	output += L"\n";
	OutputDebugStringW(output.c_str());

	return kernel;
}

void Graphics::Glow::addGlow(ID3D11DeviceContext * context, ID3D11ShaderResourceView * backBuffer, ID3D11ShaderResourceView * glowMap, ShaderResource * outputTexture)
{
	ID3D11UnorderedAccessView * nullUAV = nullptr;
	ID3D11ShaderResourceView * nullSRV = nullptr;

	context->CSSetShader(glow, nullptr, 0);
	context->CSSetUnorderedAccessViews(0, 1, glowPass0, nullptr);
	context->CSSetShaderResources(0, 1, &glowMap);
	context->Dispatch(WIN_WIDTH / 32, WIN_HEIGHT / 18, 1);


	context->CSSetUnorderedAccessViews(0, 1, glowPass1, nullptr);
	context->CSSetShaderResources(0, 1, glowPass0);

	context->CSSetShader(glow2, nullptr, 0);
	context->Dispatch(WIN_WIDTH / 32, WIN_HEIGHT / 18, 1);

	context->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);

	context->CSSetShader(merger, nullptr, 0);
	auto sampler = states->LinearWrap();
	context->CSSetSamplers(0, 1, &sampler);

	context->CSSetShaderResources(0, 1, &backBuffer);
	context->CSSetShaderResources(1, 1, glowPass1);
	context->CSSetUnorderedAccessViews(0, 1, *outputTexture, nullptr);

	context->Dispatch(WIN_WIDTH / 16, WIN_HEIGHT / 9, 1);

	context->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
	context->CSSetShaderResources(0, 1, &nullSRV);
	context->CSSetShaderResources(1, 1, &nullSRV);
}