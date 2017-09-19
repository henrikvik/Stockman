#include "Shader.h"
#include "../ThrowIfFailed.h"
#include <d3dcompiler.h>
#include <iostream>
#include <Windows.h>
#ifdef DEBUG
#define SHADER_COMPILE_FLAGS D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
#else
#define SHADER_COMPILE_FLAGS NULL
#endif

namespace Graphics
{

    Shader::Shader(ID3D11Device * device, LPCWSTR shaderPath, std::initializer_list<D3D11_INPUT_ELEMENT_DESC> inputDesc)
    {
        ID3DBlob *vsShader, *psShader, *errorMsg;

        HRESULT vshr = D3DCompileFromFile(shaderPath, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", SHADER_COMPILE_FLAGS, NULL, &vsShader, &errorMsg);
        if FAILED(vshr)
        {
            OutputDebugString((char*)errorMsg->GetBufferPointer());
            throw "Failed to compile Vertex Shader";
        }

        HRESULT pshr = D3DCompileFromFile(shaderPath, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", SHADER_COMPILE_FLAGS, NULL, &psShader, &errorMsg);
        if FAILED(pshr)
        {
            OutputDebugString((char*)errorMsg->GetBufferPointer());
            throw "Failed to compile Pixel Shader";
        }

        ThrowIfFailed(device->CreateInputLayout(inputDesc.begin(), inputDesc.size(), vsShader->GetBufferPointer(), vsShader->GetBufferSize(), &inputLayout));
        ThrowIfFailed(device->CreateVertexShader(vsShader->GetBufferPointer(), vsShader->GetBufferSize(), NULL, &vertexShader));

        ThrowIfFailed(device->CreatePixelShader(psShader->GetBufferPointer(), psShader->GetBufferSize(), NULL, &pixelShader));
    }

    Shader::~Shader()
    {
        inputLayout->Release();
        vertexShader->Release();
        pixelShader->Release();
    }

    //void Shader::setShader(ID3D11DeviceContext * deviceContext, int flags)
    //{
    //    if (flags & VS)
    //    {
    //        deviceContext->IASetInputLayout(inputLayout);
    //        deviceContext->VSSetShader(vertexShader, nullptr, 0);
    //    }
    //    if (flags & PS)
    //    {
    //        deviceContext->PSSetShader(pixelShader, nullptr, 0);
    //    }
    //}


    ComputeShader::ComputeShader(ID3D11Device * device, LPCWSTR shaderPath)
    {
        ID3DBlob *csShader, *errorMsg;

        HRESULT cshr = D3DCompileFromFile(shaderPath, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS", "cs_5_0", SHADER_COMPILE_FLAGS, NULL, &csShader, &errorMsg);
        if FAILED(cshr)
        {
            OutputDebugString((char*)errorMsg->GetBufferPointer());
            throw "Failed to compile Compute Shader";
        }

        ThrowIfFailed(device->CreateComputeShader(csShader->GetBufferPointer(), csShader->GetBufferSize(), NULL, &computeShader));
    }


    ComputeShader::~ComputeShader()
    {
        computeShader->Release();
    }

    //void ComputeShader::setShader(ID3D11DeviceContext * deviceContext)
    //{
    //    deviceContext->CSSetShader(computeShader, nullptr, 0);
    //}
}
