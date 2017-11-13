#include "ShaderResource.h"
#include "../Device.h"

ShaderResource::ShaderResource(UINT width, UINT height, DXGI_FORMAT format)
    : ShaderResource(Global::device, width, height, format)
{
}

ShaderResource::ShaderResource(ID3D11Device * device, UINT width, UINT height, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Format = format;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;

	ID3D11Texture2D* texture;
	ThrowIfFailed(device->CreateTexture2D(&textureDesc, NULL, &texture));

	D3D11_RENDER_TARGET_VIEW_DESC renderDesc = {};
	renderDesc.Format = format;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	ThrowIfFailed(device->CreateRenderTargetView(texture, &renderDesc, &renderTarget));

	D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedDesc = {};
	unorderedDesc.Format = format;
	unorderedDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	unorderedDesc.Texture2D.MipSlice = 0;

	ThrowIfFailed(device->CreateUnorderedAccessView(texture, &unorderedDesc, &unorderedAccess));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
	resourceDesc.Format = format;
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