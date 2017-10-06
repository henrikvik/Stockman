#include "HUD.H"
#include <WICTextureLoader.h>

Graphics::HUD::HUD(ID3D11Device * device, ID3D11DeviceContext * context)
:shader(device, SHADER_PATH("GUIShader.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } ,{ "ELEMENT", 0, DXGI_FORMAT_R32_UINT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA } })
{
    createHUDTextures(device, context);
    createHUDVBS(device);
    createHUDCBs(device);
    setHUDTextRenderPos();
   sFont[0] = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/comicsans.spritefont");
   sBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

Graphics::HUD::~HUD()
{
    SAFE_RELEASE(vertexBuffer);
    SAFE_RELEASE(crosshair);
    SAFE_RELEASE(HP);
    SAFE_RELEASE(HUDCBuffer);
}

void Graphics::HUD::drawHUD(ID3D11DeviceContext * context, ID3D11RenderTargetView * backBuffer, ID3D11BlendState * blendState)
{
    renderText(blendState);
    UINT stride = 20, offset = 0;
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(shader);

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT sampleMask = 0xffffffff;
    context->OMSetBlendState(blendState, blendFactor, sampleMask);
 
    context->OMSetRenderTargets(1, &backBuffer, nullptr);

    context->VSSetShader(shader, nullptr, 0);

    context->PSSetShaderResources(0, 1, &crosshair);
    context->PSSetShaderResources(1, 1, &HP);
    context->PSSetShader(shader, nullptr, 0);

    context->Draw(12, 0);
    ID3D11ShaderResourceView * SRVNULL = nullptr;
    context->PSSetShaderResources(0, 1, &SRVNULL);
}

void Graphics::HUD::queueText(Graphics::TextString * text)
{
    textQueue.push_back(*text);
}

void Graphics::HUD::fillHUDInfo(HUDInfo * info)
{
    currentInfo = info;
}

void Graphics::HUD::createHUDVBS(ID3D11Device * device)
{
    struct GUI
    {
        DirectX::SimpleMath::Vector2 verts;
        DirectX::SimpleMath::Vector2 uv;
        UINT element;
    };

    GUI GUIquad[12];
    GUIquad[0].verts = DirectX::SimpleMath::Vector2{ -0.05f, -0.05f };
    GUIquad[0].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

    GUIquad[1].verts = DirectX::SimpleMath::Vector2{ -0.05f, 0.05f };
    GUIquad[1].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[2].verts = DirectX::SimpleMath::Vector2{ 0.05f, -0.05f };
    GUIquad[2].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[3].verts = DirectX::SimpleMath::Vector2{ 0.05f, 0.05f };
    GUIquad[3].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

    GUIquad[4].verts = GUIquad[2].verts;
    GUIquad[4].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[5].verts = GUIquad[1].verts;
    GUIquad[5].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[0].element = 0;
    GUIquad[1].element = 0;
    GUIquad[2].element = 0;
    GUIquad[3].element = 0;
    GUIquad[4].element = 0;
    GUIquad[5].element = 0;


    GUIquad[6].verts = DirectX::SimpleMath::Vector2{ -1.0f, -1.0f };
    GUIquad[6].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

    GUIquad[7].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.8f };
    GUIquad[7].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[8].verts = DirectX::SimpleMath::Vector2{ -0.8f, -1.0f };
    GUIquad[8].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[9].verts = DirectX::SimpleMath::Vector2{ -0.8f, -0.8f };
    GUIquad[9].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

    GUIquad[10].verts = GUIquad[8].verts;
    GUIquad[10].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[11].verts = GUIquad[7].verts;
    GUIquad[11].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[6].element = 1;
    GUIquad[7].element = 1;
    GUIquad[8].element = 1;
    GUIquad[9].element = 1;
    GUIquad[10].element = 1;
    GUIquad[11].element = 1;

    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.ByteWidth = sizeof(GUIquad);

    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = GUIquad;


    ThrowIfFailed(device->CreateBuffer(&desc, &data, &vertexBuffer));
}

void Graphics::HUD::createHUDTextures(ID3D11Device * device, ID3D11DeviceContext * context)
{
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("crosshair.png"), nullptr, &crosshair));
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("HPbar.png"), nullptr, &HP));
}

void Graphics::HUD::renderText(ID3D11BlendState * blendState)
{
    sBatch->Begin(DirectX::SpriteSortMode_Deferred, blendState);

    for (size_t i = 0; i < textQueue.size(); i++)
    {
        TextString temp = textQueue.at(i);

            sFont[temp.font]->DrawString(sBatch.get(), temp.text.c_str(), temp.pos, temp.color);   
    }
    textQueue.clear();

    renderHUDText();

    sBatch->End();
}

void Graphics::HUD::setHUDTextRenderPos()
{
    ammoPos1 = DirectX::SimpleMath::Vector2((WIN_WIDTH / 2)  + 60 , (WIN_HEIGHT / 2) + 60);
    ammoPos2 = DirectX::SimpleMath::Vector2(WIN_WIDTH - 250, (WIN_HEIGHT / 2) + 300);
    
    scorePos = DirectX::SimpleMath::Vector2(0, 0);
    wavePos = DirectX::SimpleMath::Vector2(0, 0);
    timePos = DirectX::SimpleMath::Vector2(0, 0);
}

void Graphics::HUD::renderHUDText()
{   
    if (!currentInfo->sledge)
    {
        std::wstring temp = std::to_wstring(currentInfo->cuttleryAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->cuttleryAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos1, DirectX::Colors::Red);

        temp = std::to_wstring(currentInfo->iceAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->iceAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos2, DirectX::Colors::Red);
    }
    else
    {
        std::wstring temp = std::to_wstring(currentInfo->cuttleryAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->cuttleryAmmo[1]);

        DirectX::SimpleMath::Vector2 tempPos = ammoPos2;
        tempPos.y += 20;
        sFont[0]->DrawString(sBatch.get(), temp.c_str(), tempPos, DirectX::Colors::Red);

        temp = std::to_wstring(currentInfo->iceAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->iceAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos2, DirectX::Colors::Red);
    }
}

void Graphics::HUD::updateHUDConstant()
{
}

void Graphics::HUD::createHUDCBs(ID3D11Device * device)
{
    D3D11_BUFFER_DESC desc = { 0 };
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.ByteWidth = sizeof(float) * 4;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    
    float temp[2] = { 1.0f };
    D3D11_SUBRESOURCE_DATA data = { 0 };
    data.pSysMem = temp;

    ThrowIfFailed(device->CreateBuffer(&desc, &data, &HUDCBuffer));

}

