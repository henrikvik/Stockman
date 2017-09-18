#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include <vector>

#define NO_SHADER -1

namespace Graphics
{
    class ShaderHandler
    {
    public:
        ShaderHandler();
        ~ShaderHandler();

        int createVertexShader(ID3D11Device* device, wchar_t* name, char* entrypoint, D3D11_INPUT_ELEMENT_DESC * desc, UINT nrOfElements);
        int createGeometryShader(ID3D11Device* device, wchar_t* name, char* entrypoint);
        int createPixelhader(ID3D11Device* device, wchar_t* name, char* entrypoint);

        void setShaders(int vs, int gs, int ps, ID3D11DeviceContext* context);


    private:
        std::vector<ID3D11VertexShader*> vertexShaders;
        std::vector<ID3D11InputLayout*> inputLayouts;

        std::vector<ID3D11PixelShader*> pixelShaders;
        std::vector<ID3D11GeometryShader*> geometryShaders;

    };
}