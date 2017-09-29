#include "ShaderResource.h"

ShaderResource::ShaderResource(ID3D11Device * device, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;

	ID3D11Texture2D* texture;
	ThrowIfFailed(device->CreateTexture2D(&textureDesc, NULL, &texture));

	D3D11_RENDER_TARGET_VIEW_DESC renderDesc = {};
	renderDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	ThrowIfFailed(device->CreateRenderTargetView(texture, &renderDesc, &renderTarget));

	D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedDesc = {};
	unorderedDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	unorderedDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	unorderedDesc.Texture2D.MipSlice = 0;

	ThrowIfFailed(device->CreateUnorderedAccessView(texture, &unorderedDesc, &unorderedAccess));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MipLevels = 1;

	ThrowIfFailed(device->CreateShaderResourceView(texture, &resourceDesc, &shaderResource));

	texture->Release();
}

ShaderResource::~ShaderResource()
{
	SAFE_RELEASE(renderTarget);
	SAFE_RELEASE(shaderResource);
	SAFE_RELEASE(unorderedAccess);
}