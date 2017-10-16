#include "DoF.h"
namespace Graphics
{
    DoF::DoF(ID3D11Device * device)
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

        context->PSSetSamplers(0, 2, samplers);
        UINT stride = 16, offset = 0;
        context->IASetVertexBuffers(0,1, &vertexBuffer, &stride, &offset);
    }


    void DoF::createFullScreenQuad(ID3D11Device * device)
    {
        struct GUI
        {
            DirectX::SimpleMath::Vector2 verts;
            DirectX::SimpleMath::Vector2 uv;
        };

        GUI GUIquad[24];
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
