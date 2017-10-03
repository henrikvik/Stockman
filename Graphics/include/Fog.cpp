#include "Fog.H"

namespace Graphics
{
	constexpr int MAX_FOG_SIZE = 100;

	Fog::Fog(ID3D11Device* device)
		: fogShader(device, SHADER_PATH("Fog.hlsl"))
		, fogDataBuffer(device, CpuAccess::Write, MAX_FOG_SIZE)
	{
		fogData.push_back({ { 1,1,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,0},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,0,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 0,0,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 0,0,0},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,0},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,0},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,1},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,1,2},{ 1,0,0, 1 } });
		fogData.push_back({ { 1,2,1},{ 1,0,0, 1 } });
	}

	Fog::~Fog()
	{
	}

	void Fog::renderFog(ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer)
	{
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);

		deviceContext->IASetInputLayout(nullptr);
		deviceContext->VSSetShader(fogShader, nullptr, 0);
		deviceContext->PSSetShader(fogShader, nullptr, 0);

		fogDataBuffer.write(deviceContext, fogData.data(), fogData.size() * sizeof(FogData));


		deviceContext->VSSetShaderResources(0, 1, fogDataBuffer);
		deviceContext->Draw(fogData.size(), 0);
	}
}