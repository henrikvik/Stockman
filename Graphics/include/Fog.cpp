#include "Fog.H"
#include <Graphics\include\Device.h>
#include <Graphics\include\CommonState.h>
#include<Graphics\include\MainCamera.h>

namespace Graphics
{
	constexpr int MAX_FOG_SIZE = 100;
	constexpr float FOG_COLOR = 0.3f;
	Fog::Fog(ID3D11Device* device)
		: fogShader(device, SHADER_PATH("Fog.hlsl"))
		, fogDataBuffer(device, CpuAccess::Write, MAX_FOG_SIZE)
		, fullScreen(Resources::Shaders::FullscreenQuad)
	{
		//*
		fogData.push_back({ { -1000, 3,-1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		fogData.push_back({ { 1000,  3,-1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		fogData.push_back({ { 1000,  3, 1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });								 
		fogData.push_back({ { 1000,  3,1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		fogData.push_back({ { -1000, 3,1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		fogData.push_back({ { -1000, 3,-1000 }, { FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		//*/						 

		fogDataBuffer.write(Global::context, fogData.data(), fogData.size() * sizeof(FogData));
	}

	Fog::~Fog()
	{
	}

	void Fog::renderFog(ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, ID3D11ShaderResourceView* depthMap)const
	{
		ID3D11Buffer* camBuffer = *Global::mainCamera->getBuffer();
		ID3D11Buffer* invBuffer = *Global::mainCamera->getInverseBuffer();
		deviceContext->VSSetConstantBuffers(0,1, &camBuffer);
		deviceContext->VSSetConstantBuffers(1, 1, &invBuffer);

		deviceContext->PSSetConstantBuffers(0, 1, &camBuffer);
		deviceContext->PSSetConstantBuffers(1, 1, &invBuffer);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		auto sampler = Global::cStates->LinearClamp();
		Global::context->PSSetSamplers(0, 1, &sampler);
		deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);
		static float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		static UINT sampleMask = 0xffffffff;
		Global::context->OMSetBlendState(Global::transparencyBlendState, blendFactor, sampleMask); 
		deviceContext->IASetInputLayout(nullptr);
		deviceContext->RSSetState(Global::cStates->CullNone());
		deviceContext->VSSetShader(fogShader, nullptr, 0);
		deviceContext->PSSetShader(fogShader, nullptr, 0);

		deviceContext->VSSetShaderResources(0, 1, fogDataBuffer); 
		deviceContext->PSSetShaderResources(1, 1, &depthMap);
		deviceContext->Draw((UINT)fogData.size(), 0);

        ID3D11ShaderResourceView* null = nullptr;
        deviceContext->PSSetShaderResources(1, 1, &null);

		Global::context->OMSetBlendState(Global::cStates->Opaque(), blendFactor, sampleMask);
    }
}