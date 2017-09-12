#include "Shader.h"
#include <ThrowIfFailed.h>
#include <d3dcompiler.h>

namespace Graphics
{

    Shader::Shader(ID3D11Device * device, LPCWSTR shaderPath, std::initializer_list<D3D11_INPUT_ELEMENT_DESC> inputDesc)
    {
    #ifdef DEBUG
    #define SHADER_COMPILE_FLAGS D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
    #else
    #define SHADER_COMPILE_FLAGS NULL
    #endif

        ID3DBlob *vsShader, *psShader;

        ThrowIfFailed(D3DCompileFromFile(shaderPath, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", SHADER_COMPILE_FLAGS, NULL, &vsShader, nullptr));
        ThrowIfFailed(device->CreateInputLayout(inputDesc.begin(), inputDesc.size(), vsShader->GetBufferPointer(), vsShader->GetBufferSize(), &inputLayout));
        ThrowIfFailed(device->CreateVertexShader(vsShader->GetBufferPointer(), vsShader->GetBufferSize(), NULL, &vertexShader));

        ThrowIfFailed(D3DCompileFromFile(shaderPath, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", SHADER_COMPILE_FLAGS, NULL, &psShader, nullptr));
        ThrowIfFailed(device->CreatePixelShader(psShader->GetBufferPointer(), psShader->GetBufferSize(), NULL, &pixelShader));
    }

    Shader::~Shader()
    {
        inputLayout->Release();
        vertexShader->Release();
        pixelShader->Release();
    }

    void Shader::setShader(ID3D11DeviceContext * deviceContext, int flags)
    {
        if (flags & VS)
        {
            deviceContext->IASetInputLayout(inputLayout);
            deviceContext->VSSetShader(vertexShader, nullptr, 0);
        }
        if (flags & PS)
        {
            deviceContext->PSSetShader(pixelShader, nullptr, 0);
        }
    }
}
