#include "SkyRenderer.h"
#include <Engine\Constants.h>
#include <DDSTextureLoader.h>
#include "ThrowIfFailed.h"
#include <Engine\Profiler.h>

namespace Graphics 
{
    SkyRenderer::SkyRenderer(ID3D11Device * device, int shadowRes, HybrisLoader::HybrisLoader & hybrisLoader) :
		shader(device, Resources::Shaders::SkyShader),
		sphereTransformBuffer(device),
		shadowDepthStencil(device, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION),
		sun(device, shadowRes, shadowRes)
	{
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, TEXTURE_PATH("skyBox.dds"), nullptr, &srv));
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, TEXTURE_PATH("skyboxgradient.dds"), nullptr, &srv2));
		createSampler(device);

		this->skySphere = &hybrisLoader.getModel(Resources::Models::SkySphere)->getMesh();
	}

	SkyRenderer::~SkyRenderer()
	{
		SAFE_RELEASE(shadowSampler);
		SAFE_RELEASE(srv);
		SAFE_RELEASE(srv2);
	}

	void SkyRenderer::renderSky(ID3D11DeviceContext * context, Graphics::Camera * cam)
	{
		context->IASetInputLayout(nullptr);		
		context->VSSetShader(shader, nullptr, 0);
		context->PSSetShader(shader, nullptr, 0);
        context->VSSetShaderResources(0, 1, skySphere->getVertexBuffer());

		context->VSSetConstantBuffers(0, 1, *cam->getBuffer());
		context->VSSetConstantBuffers(4, 1, sphereTransformBuffer);

		context->PSSetConstantBuffers(1, 1, *sun.getShaderBuffer());

		context->Draw(skySphere->getVertexCount(), 0);
	}

	void SkyRenderer::update(ID3D11DeviceContext * context, float deltaTime, DirectX::SimpleMath::Vector3 pos)
	{
		float radiansPerSecond = 0.01745f * deltaTime * 0.005f;

		sun.update(context, radiansPerSecond, pos);

        DirectX::SimpleMath::Matrix temp = DirectX::SimpleMath::Matrix::CreateTranslation(pos);

		D3D11_MAPPED_SUBRESOURCE data = {};

		context->Map(sphereTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

		memcpy(data.pData, &temp, sizeof(DirectX::SimpleMath::Matrix));

		context->Unmap(sphereTransformBuffer, 0);
	}

	void SkyRenderer::drawShadows(ID3D11DeviceContext * context, Graphics::Shader * shader)
	{

		PROFILE_BEGIN("SetupShaders");
		context->RSSetViewports(1, &sun.getViewPort());
		context->IASetInputLayout(nullptr);
		context->VSSetShader(*shader, nullptr, 0);
		context->PSSetShader(nullptr, nullptr, 0);
		context->OMSetRenderTargets(0, nullptr, shadowDepthStencil);

		context->VSSetConstantBuffers(0, 1, *sun.getMatrixBuffer());
		PROFILE_END();
	}

	void SkyRenderer::clear(ID3D11DeviceContext * context)
	{
		context->ClearDepthStencilView(shadowDepthStencil, D3D11_CLEAR_DEPTH, 1.f, 0);
	}

	void SkyRenderer::createSampler(ID3D11Device * device)
	{
		D3D11_SAMPLER_DESC sDesc = {};
		sDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		sDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		sDesc.BorderColor[0] = 1;
		sDesc.BorderColor[1] = 1;
		sDesc.BorderColor[2] = 1;
		sDesc.BorderColor[3] = 1;
		sDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		sDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		sDesc.MaxAnisotropy = 0;
		sDesc.MinLOD = 0;
		sDesc.MaxLOD = D3D11_FLOAT32_MAX;
		sDesc.MipLODBias = 0;

		ThrowIfFailed(device->CreateSamplerState(&sDesc, &shadowSampler));
	}
}