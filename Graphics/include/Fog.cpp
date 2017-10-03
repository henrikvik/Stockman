#include "Fog.H"

const int maxFogSize = 100;

Fog::Fog(ID3D11Device* device)
	: fogShader(device, SHADER_PATH("FogShader.hlsl"))
	, fogDataBuffer(device, CpuAccess::Write, maxFogSize)
{
	fogVec.push_back({ { 1,1,1 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,0 },{ 1,0,0 } });
	fogVec.push_back({ { 1,0,1 },{ 1,0,0 } });
	fogVec.push_back({ { 0,0,1 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,1 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,1 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,1 },{ 1,0,0 } });
	fogVec.push_back({ { 0,0,0 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,1 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,1 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,0 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,1 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,0 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,1 },{ 1,0,0 } });
	fogVec.push_back({ { 1,1,2 },{ 1,0,0 } });
	fogVec.push_back({ {1,2,1}, {1,0,0} });
}

Fog::~Fog()
{
}

void Fog::renderFog(ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);

	deviceContext->IASetInputLayout(nullptr);
	deviceContext->VSSetShader(fogShader, nullptr, 0);
	deviceContext->PSSetShader(fogShader, nullptr, 0);

	fogDataBuffer.write(deviceContext, fogVec.data(), fogVec.size() * sizeof(FogData));


	deviceContext->VSSetShaderResources(0, 1, fogDataBuffer);
	deviceContext->Draw(fogVec.size(), 0);
}
