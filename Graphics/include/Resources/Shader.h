#pragma once
#include <d3d11.h>
#include <initializer_list>

namespace Graphics
{
    class Shader
    {
    public:
        enum Flags 
        {
            VS = 1 << 0,
            PS = 1 << 1
        };

        Shader(ID3D11Device * device, LPCWSTR shaderPath, std::initializer_list<D3D11_INPUT_ELEMENT_DESC> inputDesc);
        virtual ~Shader();

        // second argument is flags for which shaders to bind bitwise from Shader::Flags enum
        void setShader(ID3D11DeviceContext * deviceContext, int flags = VS | PS);

        operator ID3D11InputLayout  *() { return inputLayout;  }
        operator ID3D11VertexShader *() { return vertexShader; }
        operator ID3D11PixelShader  *() { return pixelShader;  }
    private:
        ID3D11InputLayout  * inputLayout;
        ID3D11VertexShader * vertexShader;
        ID3D11PixelShader  * pixelShader;
    };

    class ComputeShader
    {
    public:
        ComputeShader(ID3D11Device * device, LPCWSTR shaderPath);
        virtual ~ComputeShader();

        void setShader(ID3D11DeviceContext * deviceContext);

    private:
        ID3D11ComputeShader * computeShader;
    };
}