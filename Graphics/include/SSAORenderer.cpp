#include "SSAORenderer.h"
#include <Engine\Constants.h>
#include <WICTextureLoader.h>
#include "CommonStates.h"

namespace Graphics 
{
	SSAORenderer::SSAORenderer(ID3D11Device * device):
		normalTexture(device, WIN_WIDTH, WIN_HEIGHT),
		ssaoOutput(device, WIN_WIDTH / 2, WIN_HEIGHT / 2, DXGI_FORMAT_R8_UNORM),
		ssaoOutputSwap(device, WIN_WIDTH / 2, WIN_HEIGHT / 2, DXGI_FORMAT_R8_UNORM),
		ssaoShader(device, SHADER_PATH("SSAOShaders/SSAOComputeShader.hlsl")),
		blurHorizontal(device, SHADER_PATH("SSAOShaders/GaussianBlurHorizontal.hlsl")),
		blurVertical(device, SHADER_PATH("SSAOShaders/GaussianBlurVertical.hlsl")),
		ssaoMerger(device, SHADER_PATH("SSAOShaders/SSAOMerger.hlsl"))
	{
		DirectX::CreateWICTextureFromFile(device, TEXTURE_PATH("randomNormals.jpg"), NULL, &randomNormals);
	}

	SSAORenderer::~SSAORenderer()
	{
		SAFE_RELEASE(randomNormals);
	}

	void SSAORenderer::renderSSAO(ID3D11DeviceContext * context, Camera * camera, DepthStencil * depthBuffer, ShaderResource * inputBackBuffer, ShaderResource * outputBackBuffer)
	{
		static float clear[4] = { 0 };
		context->ClearUnorderedAccessViewFloat(ssaoOutput, clear);

		ID3D11SamplerState * samplers[] =
		{
            Global::cStates->PointClamp(),
            Global::cStates->PointWrap()
		};
		context->CSSetSamplers(0, 2, samplers);

		context->CSSetShader(ssaoShader, nullptr, 0);
		ID3D11ShaderResourceView * srvs[] =
		{
			*depthBuffer,
			randomNormals,
			normalTexture
		};

		context->CSSetShaderResources(0, 3, srvs);
		context->CSSetUnorderedAccessViews(0, 1, ssaoOutput, nullptr);
		context->CSSetConstantBuffers(0, 1, *camera->getInverseBuffer());
		context->Dispatch(40, 23, 1);

		ID3D11ShaderResourceView * srvsNULL[3] = { nullptr };
		context->CSSetShaderResources(0, 3, srvsNULL);
		ID3D11ShaderResourceView * srvNULL = nullptr;

		//blur the occlusion map in two passes
		context->CSSetShader(blurHorizontal, nullptr, 0);
		context->CSSetUnorderedAccessViews(0, 1, ssaoOutputSwap, nullptr);
		context->CSSetShaderResources(0, 1, ssaoOutput);
		context->Dispatch(40, 23, 1);

		context->CSSetShaderResources(0, 1, &srvNULL);

		context->CSSetShader(blurVertical, nullptr, 0);
		context->CSSetUnorderedAccessViews(0, 1, ssaoOutput, nullptr);
		context->CSSetShaderResources(0, 1, ssaoOutputSwap);
		context->Dispatch(40, 23, 1);

		//Merge the blurred occlusion map with back buffer
		context->CSSetShader(ssaoMerger, nullptr, 0);
		context->CSSetUnorderedAccessViews(0, 1, *outputBackBuffer, nullptr);
		context->CSSetShaderResources(0, 1, *inputBackBuffer);
		context->CSSetShaderResources(1, 1, ssaoOutput);
		context->Dispatch(80, 45, 1);


		context->CSSetShaderResources(0, 1, &srvNULL);
		context->CSSetShaderResources(1, 1, &srvNULL);
		ID3D11UnorderedAccessView * uavnull = nullptr;
		context->CSSetUnorderedAccessViews(0, 1, &uavnull, nullptr);

		static float blackClearColor[4] = {0, 0, 0, 0};
		context->ClearRenderTargetView(normalTexture, blackClearColor);
	}

}