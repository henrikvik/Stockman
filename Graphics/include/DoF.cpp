#include "DoF.h"
namespace Graphics
{
    DoF::DoF(ID3D11Device * device):
        blur1col0(device, WIN_WIDTH, WIN_HEIGHT),
        blur1col1(device, WIN_WIDTH, WIN_HEIGHT),
        blur2Final(device, WIN_WIDTH, WIN_HEIGHT),
        CoCcreation(device, SHADER_PATH("DoFShaders/CreateCoc.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } }),
        blur1(device, SHADER_PATH("DoFShaders/Blur1.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } }),
        blur2(device, SHADER_PATH("DoFShaders/Blur2.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } }),
        glue(device, SHADER_PATH("DoFShaders/Glue.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } })
    {

        states = new DirectX::CommonStates(device);
        //samplers[0] = states->PointClamp();
        //samplers[1] = states->LinearClamp();

    }

    DoF::~DoF()
    {
        //SAFE_RELEASE(samplers[0]);
        //SAFE_RELEASE(samplers[1]);
        delete states;
    }

    void DoF::DoFRender(ID3D11DeviceContext * context, ShaderResource * colorBuffer, DepthStencil * depthBuffer, ShaderResource * outputBuffer, Camera *cam)
    {
        static ID3D11SamplerState * samplers[] = {
            states->PointClamp(),
            states->LinearClamp()
        };

        
        static UINT stride = 16, offset = 0;
        context->IASetVertexBuffers(0,1, &vertexBuffer, &stride, &offset);


        //calculate CoC pass
        ID3D11ShaderResourceView * srv[] =
        {
            *colorBuffer,
            *depthBuffer
        };
        context->PSSetSamplers(0, 2, samplers);
        context->PSSetShaderResources(0, 2, srv);

        context->OMSetRenderTargets(1, blur2Final, nullptr);

        static ID3D11RenderTargetView * rtvNULL = nullptr;
        context->OMSetRenderTargets(1, &rtvNULL, nullptr);

        //blur pass 1
        context->PSSetShaderResources(0, 1, blur2Final);



        ID3D11RenderTargetView * rtv[] =
        {
            blur1col0,
            blur1col1
        };
        context->OMSetRenderTargets(2, rtv, nullptr);




        static ID3D11ShaderResourceView * srvNULL = nullptr;
        context->PSSetShaderResources(0, 1, &srvNULL);
        static ID3D11RenderTargetView * rtvNULLAr[] =
        {
            nullptr,
            nullptr
        };
        context->OMSetRenderTargets(2, rtvNULLAr, nullptr);


        //blur pass 2
        context->PSSetShaderResources(0, 1, blur1col0);
        context->PSSetShaderResources(1, 1, blur1col1);


        context->OMSetRenderTargets(1, blur2Final, nullptr);


        context->OMSetRenderTargets(1, &rtvNULL, nullptr);
        static ID3D11ShaderResourceView * srvNULLAr[] =
        {
            nullptr,
            nullptr
        };
        context->PSSetShaderResources(0, 2, srvNULLAr);

        //glue pass
        context->PSSetShaderResources(0, 1, blur2Final);

        context->OMSetRenderTargets(1, *outputBuffer, nullptr);

        context->OMSetRenderTargets(1, &rtvNULL, nullptr);
        context->PSSetShaderResources(0, 1, &srvNULL);
    }


    void DoF::createFullScreenQuad(ID3D11Device * device)
    {
        struct GUI
        {
            DirectX::SimpleMath::Vector2 verts;
            DirectX::SimpleMath::Vector2 uv;
        };

        GUI GUIquad[6];
        GUIquad[0].verts = DirectX::SimpleMath::Vector2{ -1.0f, -1.0f };
        GUIquad[0].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

        GUIquad[1].verts = DirectX::SimpleMath::Vector2{ -1.0f, 1.0f };
        GUIquad[1].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

        GUIquad[2].verts = DirectX::SimpleMath::Vector2{ 1.0f, -1.0f };
        GUIquad[2].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

        GUIquad[3].verts = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };
        GUIquad[3].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

        GUIquad[4].verts = GUIquad[2].verts;
        GUIquad[4].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

        GUIquad[5].verts = GUIquad[1].verts;
        GUIquad[5].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.ByteWidth = sizeof(GUIquad);

        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
        data.pSysMem = GUIquad;


        ThrowIfFailed(device->CreateBuffer(&desc, &data, &vertexBuffer));
    }
}
