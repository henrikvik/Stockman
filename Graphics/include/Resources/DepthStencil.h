#pragma once
#include <d3d11.h>

namespace Graphics
{
    class DepthStencil
    {
    public:
        DepthStencil(ID3D11Device * device, UINT width, UINT height);
        virtual ~DepthStencil();

        void clear(ID3D11DeviceContext * context, UINT clearFlags = D3D11_CLEAR_DEPTH, FLOAT depth = 1.0f, UINT8 stencil = 0);

        ID3D11DepthStencilView * getBuffer();
        ID3D11ShaderResourceView * getResource();
    private:
        
        ID3D11DepthStencilView * depthBuffer;
        ID3D11ShaderResourceView * depthView;
    };
}