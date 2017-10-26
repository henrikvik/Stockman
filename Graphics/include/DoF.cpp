#include "DoF.h"
namespace Graphics
{
    DoF::DoF(ID3D11Device * device):
        blur1col0(device, WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_SNORM),
        blur1col1(device, WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_SNORM),
        blur2Final(device, WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_SNORM),
        //renderTarget(device, WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R16G16B16A16_SNORM),
        cbuffer(device),
        CoCcreation(device, SHADER_PATH("DoFShaders/CreateCoc.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } }),
        blur1(device, SHADER_PATH("DoFShaders/Blur1.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } }),
        blur2(device, SHADER_PATH("DoFShaders/Blur2.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } }),
        glue(device, SHADER_PATH("DoFShaders/Glue.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } })
    {

        states = new DirectX::CommonStates(device);
        //samplers[0] = states->PointClamp();
        //samplers[1] = states->LinearClamp();
        createFullScreenQuad(device);
        firstTime = true;
    }

    DoF::~DoF()
    {
        //SAFE_RELEASE(samplers[0]);
        //SAFE_RELEASE(samplers[1]);
        SAFE_RELEASE(vertexBuffer);
        delete states;
    }

    void DoF::DoFRender(ID3D11DeviceContext * context, ShaderResource * colorBuffer, DepthStencil * depthBuffer, ShaderResource * outputBuffer, Camera *cam)
    {
        if (firstTime)
        {
            updateCoc(context);
            firstTime = false;
        }
        static ID3D11SamplerState * samplers[] = {
            states->PointClamp(),
            states->LinearClamp()
        };

        
        static UINT stride = 16, offset = 0;
        context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->IASetInputLayout(CoCcreation);
        float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        UINT sampleMask = 0xffffffff;
        context->OMSetBlendState(nullptr, blendFactor, sampleMask);

        //calculate CoC pass
        ID3D11ShaderResourceView * srv[] =
        {
            *colorBuffer,
            *depthBuffer
        };
        context->PSSetConstantBuffers(0, 1, cbuffer);
        context->VSSetShader(CoCcreation, nullptr, 0);
        context->PSSetShader(CoCcreation, nullptr, 0);

        context->PSSetSamplers(0, 2, samplers);
        context->PSSetShaderResources(0, 2, srv);

        context->OMSetRenderTargets(1, blur2Final, nullptr);
        context->Draw(6, 0);

        static ID3D11RenderTargetView * rtvNULL = nullptr;
        context->OMSetRenderTargets(1, &rtvNULL, nullptr);

        //blur pass 1
        context->PSSetShaderResources(0, 1, blur2Final);


        context->PSSetShader(blur1, nullptr, 0);
        

        ID3D11RenderTargetView * rtv[] =
        {
            blur1col0,
            blur1col1
        };
        context->OMSetRenderTargets(2, rtv, nullptr);

        context->Draw(6, 0);


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

        context->PSSetShader(blur2, nullptr, 0);
        context->OMSetRenderTargets(1, blur2Final, nullptr);

        context->Draw(6, 0);
        context->OMSetRenderTargets(1, &rtvNULL, nullptr);
        static ID3D11ShaderResourceView * srvNULLAr[] =
        {
            nullptr,
            nullptr
        };
        context->PSSetShaderResources(0, 2, srvNULLAr);

        //glue pass
        context->PSSetShaderResources(0, 1, blur2Final);
        context->PSSetShader(glue, nullptr, 0);



        context->OMSetRenderTargets(1, *outputBuffer, nullptr);
        context->Draw(6, 0);
        context->OMSetRenderTargets(1, &rtvNULL, nullptr);
        context->PSSetShaderResources(0, 1, &srvNULL);
    }

    //updates the values that create the circle of confusion
    void DoF::updateCoc(ID3D11DeviceContext *context, float focalLength, float focalPlane, float apature)
    {
        //change these if near and far changes
        float nearP = 0.1f;
        float farP = 250.0f;
        float focalPlaneR = focalPlane * farP;

        float CoC[2] = {
            ((apature * focalLength * focalPlaneR * (farP - nearP)) / ((focalPlaneR - focalLength) * nearP * farP)), //scale
            ((apature * focalLength * (nearP - focalPlaneR)) / ((focalPlaneR - focalLength) * nearP))    //bias
        };
        
        cbuffer.write(context, CoC, sizeof(float) * 2);

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
