#include "Glow.h"
#include <WICTextureLoader.h>
#include "ThrowIfFailed.h"
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Engine\DebugWindow.h>
#define KERNELSIZE 9
#define SIGMA 5

Graphics::Glow::Glow(ID3D11Device * device, ID3D11DeviceContext * context)
	: glow(device, SHADER_PATH("GlowShaders/GlowBlurHorizontal.hlsl"), {})
	, glow2(device, SHADER_PATH("GlowShaders/GlowBlurVertical.hlsl"), {})
	, merger(device, SHADER_PATH("GlowShaders/Merger.hlsl"), {})
	, mipGenerator(device, SHADER_PATH("GlowShaders/GlowDownSampler.hlsl"), {})
	, mipCombinder(device, SHADER_PATH("GlowShaders/GlowMipCombinder.hlsl"), {})
	, glowPass0(device, WIN_WIDTH, WIN_HEIGHT)
	, glowPass1(device, WIN_WIDTH, WIN_HEIGHT)
{
	this->states = new DirectX::CommonStates(device);

	DebugWindow *debugWindow = DebugWindow::getInstance();
	createMips(device);
}

Graphics::Glow::~Glow()
{
	delete states;
	for (int i = 0; i < MIP_LEVELS; i++)
	{
		SAFE_RELEASE(srvs[i]);
		SAFE_RELEASE(rtvs[i]);
	}

	SAFE_RELEASE(srvAllMips);

}

//Not really used since we have a standalone program now
std::vector<float> Graphics::Glow::generateKernel(int kernelSize, float sigma) 
{
	if (kernelSize % 2 == 0) kernelSize--;
	
	std::vector<float> kernel(kernelSize);

	auto g = [&](int x) -> float {
		static float gMult = 1.0f / (sqrtf(2.0f * (float)M_PI) * sigma);
		return gMult * powf((float)M_E, -(powf((float)x, 2) / (2.0f * powf(sigma, 2))));
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

void Graphics::Glow::recompileGlow(ID3D11Device * device)
{
	merger.recompile(device, SHADER_PATH("GlowShaders/Merger.hlsl"));
	glow.recompile(device, SHADER_PATH("GlowShaders/glowBlurVertical.hlsl"));
	glow2.recompile(device, SHADER_PATH("GlowShaders/GlowBlurHorizontal.hlsl"));
}

void Graphics::Glow::clear(ID3D11DeviceContext * context, float color[4])
{
	for (int i = 0; i < MIP_LEVELS; i++)
	{
		context->ClearRenderTargetView(rtvs[i], color);
	}
}

void Graphics::Glow::createMips(ID3D11Device * device)
{
	ID3D11Texture2D * texture;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = WIN_WIDTH;
	textureDesc.Height = WIN_HEIGHT;
	textureDesc.MipLevels = MIP_LEVELS;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;

	ThrowIfFailed(device->CreateTexture2D(&textureDesc, 0, &texture));

	D3D11_RENDER_TARGET_VIEW_DESC renderDesc = {};
	renderDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < MIP_LEVELS; i++)
	{
		resourceDesc.Texture2D.MostDetailedMip = i;
		renderDesc.Texture2D.MipSlice = i;

		ThrowIfFailed(device->CreateRenderTargetView(texture, &renderDesc, &this->rtvs[i]));
		ThrowIfFailed(device->CreateShaderResourceView(texture, &resourceDesc, &this->srvs[i]));
	}

	resourceDesc.Texture2D.MipLevels = MIP_LEVELS;
	resourceDesc.Texture2D.MostDetailedMip = 0;

	ThrowIfFailed(device->CreateShaderResourceView(texture, &resourceDesc, &srvAllMips));

	SAFE_RELEASE(texture);
}

void Graphics::Glow::setMipViewPort(ID3D11DeviceContext * context, int level)
{
	level = (int)powf(2.f, (float)level);
	D3D11_VIEWPORT viewPort = {};
	viewPort.Height = float(WIN_HEIGHT / level);
	viewPort.Width = float(WIN_WIDTH / level);
	viewPort.MaxDepth = 1.f;

	context->RSSetViewports(1, &viewPort);

}

void Graphics::Glow::addGlow(ID3D11DeviceContext * context, ID3D11ShaderResourceView * backBuffer, ShaderResource * outputTexture)
{
	ID3D11RenderTargetView * nullRTV = nullptr;
	ID3D11ShaderResourceView * nullSRV = nullptr;


	context->IASetInputLayout(nullptr);
	context->VSSetShader(mipGenerator, nullptr, 0);
	context->PSSetShader(mipGenerator, nullptr, 0);

	auto sampler = states->LinearWrap();
	context->PSSetSamplers(0, 1, &sampler);

	for (int i = 0; i < MIP_LEVELS - 1; i++)
	{
		context->PSSetShaderResources(0, 1, &srvs[i]);
		context->OMSetRenderTargets(1, &rtvs[i + 1], nullptr);

		setMipViewPort(context, i + 1);

		context->Draw(3, 0);

		context->OMSetRenderTargets(1, &nullRTV, nullptr);
		context->PSSetShaderResources(0, 1, &nullSRV);
	}

	context->PSSetShader(mipCombinder, nullptr, 0);
	context->OMSetRenderTargets(1, glowPass1, nullptr);
	context->PSSetShaderResources(0, 1, &srvAllMips);
	setMipViewPort(context, 0);

	context->Draw(3, 0);

	context->OMSetRenderTargets(1, &nullRTV, nullptr);
	context->PSSetShaderResources(0, 1, &nullSRV);
	//First pass


	context->IASetInputLayout(nullptr);
	context->VSSetShader(glow, nullptr, 0);
	context->PSSetShader(glow, nullptr, 0);
	context->OMSetRenderTargets(1, glowPass0, nullptr);
	context->PSSetShaderResources(0, 1, glowPass1);
	context->Draw(3, 0);

	context->OMSetRenderTargets(1, &nullRTV, nullptr);
	context->PSSetShaderResources(0, 1, &nullSRV);

	context->OMSetRenderTargets(1, glowPass1, nullptr);
	context->PSSetShaderResources(0, 1, glowPass0);

	context->VSSetShader(glow2, nullptr, 0);
	context->PSSetShader(glow2, nullptr, 0);
	context->Draw(3, 0);

	context->OMSetRenderTargets(1, &nullRTV, nullptr);
	context->PSSetShaderResources(0, 1, &nullSRV);
	


	context->VSSetShader(glow, nullptr, 0);
	context->PSSetShader(merger, nullptr, 0);
	

	context->PSSetShaderResources(4, 1, &backBuffer);
	context->PSSetShaderResources(5, 1, glowPass1);
	context->OMSetRenderTargets(1, *outputTexture, nullptr);

	context->Draw(3, 0);

	context->OMSetRenderTargets(1, &nullRTV, nullptr);
	context->PSSetShaderResources(4, 1, &nullSRV);
	context->PSSetShaderResources(5, 1, &nullSRV);


}