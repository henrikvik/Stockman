#include "DepthStencil.h"
#include "ThrowIfFailed.h"

namespace Graphics
{

    DepthStencil::DepthStencil(ID3D11Device * device, UINT width, UINT height)
    {
        D3D11_TEXTURE2D_DESC depthTexDesc = {};
        depthTexDesc.Width = width;
        depthTexDesc.Height = height;
        depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
        depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        depthTexDesc.SampleDesc.Count = 1;
        depthTexDesc.ArraySize = 1;

        D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc = {};
        depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthDesc.Texture2D.MipSlice = 0;
        depthDesc.Flags = 0;

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
        viewDesc.Format = DXGI_FORMAT_R32_FLOAT;
        viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipLevels = 1;

        ID3D11Texture2D * depthTexture;
        ThrowIfFailed(device->CreateTexture2D(&depthTexDesc, nullptr, &depthTexture));
        ThrowIfFailed(device->CreateDepthStencilView(depthTexture, &depthDesc, &depthBuffer));
        ThrowIfFailed(device->CreateShaderResourceView(depthTexture, &viewDesc, &depthView));

        depthTexture->Release();

    }

    DepthStencil::~DepthStencil()
    {
        depthBuffer->Release();
        depthView->Release();
    }

    void DepthStencil::clear(ID3D11DeviceContext * context, UINT clearFlags, FLOAT depth, UINT8 stencil)
    {
        context->ClearDepthStencilView(depthBuffer, clearFlags, depth, stencil);
    }

    ID3D11DepthStencilView * DepthStencil::getBuffer()
    {
        return depthBuffer;
    }

    ID3D11ShaderResourceView * DepthStencil::getResource()
    {
        return depthView;
    }
}