#pragma once
#include <d3d11.h>

namespace Graphics
{
    class DepthStencil
    {
    public:
        DepthStencil(ID3D11Device * device, UINT width, UINT height);
        virtual ~DepthStencil();

        inline ID3D11DepthStencilView * getBuffer() { return depthStencil; }
        inline ID3D11ShaderResourceView * getResource() { return shaderResource; }

        operator ID3D11DepthStencilView*() const { return depthStencil; }
        operator ID3D11ShaderResourceView*() const { return shaderResource; }

    private:
        
        ID3D11DepthStencilView * depthStencil;
        ID3D11ShaderResourceView * shaderResource;
    };
}