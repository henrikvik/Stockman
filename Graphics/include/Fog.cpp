#include "Fog.H"

namespace Graphics
{
	constexpr int MAX_FOG_SIZE = 100;
	constexpr float FOG_COLOR = 0.3;
	Fog::Fog(ID3D11Device* device, ID3D11DeviceContext * deviceContext )
		: fogShader(device, SHADER_PATH("Fog.hlsl"))
		, fogDataBuffer(device, CpuAccess::Write, MAX_FOG_SIZE)
	{
		//*
		fogData.push_back({ { -1000, 2,-1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		fogData.push_back({ { 1000,  2,-1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		fogData.push_back({ { 1000,  2, 1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
									
		fogData.push_back({ { 1000,  2,1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		fogData.push_back({ { -1000, 2,1000 },{ FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		fogData.push_back({ { -1000, 2,-1000 }, { FOG_COLOR,FOG_COLOR,FOG_COLOR, 1 } });
		//*/

		fogDataBuffer.write(deviceContext, fogData.data(), fogData.size() * sizeof(FogData));
	}

	Fog::~Fog()
	{
	}

	void Fog::renderFog(ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, ID3D11ShaderResourceView* worldPosMap)
	{
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);

		deviceContext->IASetInputLayout(nullptr);
		deviceContext->VSSetShader(fogShader, nullptr, 0);
		deviceContext->PSSetShader(fogShader, nullptr, 0);



		deviceContext->VSSetShaderResources(0, 1, fogDataBuffer); 
		deviceContext->PSSetShaderResources(1, 1, &worldPosMap);
		deviceContext->Draw(fogData.size(), 0);

		ID3D11RenderTargetView * nullRTV = nullptr;
		deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
	}
}