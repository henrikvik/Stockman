#include "DepthStencil.h"
#include "../ThrowIfFailed.h"

namespace Graphics
{

    DepthStencil::DepthStencil(ID3D11Device * device, UINT width, UINT height)
    {
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.SampleDesc.Count = 1;

        ID3D11Texture2D* texture;
        ThrowIfFailed(device->CreateTexture2D(&textureDesc, NULL, &texture));

        D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc = {};
        stencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
        stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

        ThrowIfFailed(device->CreateDepthStencilView(texture, &stencilDesc, &depthStencil));

        D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
        resourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
        resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        resourceDesc.Texture2D.MipLevels = 1;

        ThrowIfFailed(device->CreateShaderResourceView(texture, &resourceDesc, &shaderResource));

        texture->Release();
    }

    DepthStencil::~DepthStencil()
    {
        depthStencil->Release();
        shaderResource->Release();
    }
}