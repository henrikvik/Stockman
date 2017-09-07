#include "..\include\ResourceManager.h"

ResourceManager::ResourceManager(ID3D11Device * device)
{
	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	shaderHandler.createVertexShader(device, L"FullscreenQuad.hlsl", "VS", desc, ARRAYSIZE(desc));
	shaderHandler.createPixelhader(device, L"FullscreenQuad.hlsl", "PS");

	D3D11_INPUT_ELEMENT_DESC descDeffered[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "MATERIAL", 0, DXGI_FORMAT_R32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	shaderHandler.createVertexShader(device, L"Deffered.hlsl", "VS", descDeffered, ARRAYSIZE(descDeffered));
	shaderHandler.createPixelhader(device, L"Deffered.hlsl", "PS");

}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::setShaders(VertexShaderID vertexID, GeometryShaderID geometryID, PixelShaderID pixelID, ID3D11DeviceContext* context)
{
	this->shaderHandler.setShaders(vertexID, geometryID, pixelID, context);
}

void ResourceManager::setShaders(VertexShaderID vertexID, PixelShaderID pixelID, ID3D11DeviceContext * context)
{
	this->shaderHandler.setShaders(vertexID, NO_SHADER, pixelID, context);
}

//TODO: EVERYTHING
void ResourceManager::setComputeShader(ComputeShaderID id)
{

}
