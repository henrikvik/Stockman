#include "HUD.H"
#include <WICTextureLoader.h>

Graphics::HUD::HUD(ID3D11Device * device, ID3D11DeviceContext * context)
:shader(device, SHADER_PATH("GUIShader.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } ,{ "ELEMENT", 0, DXGI_FORMAT_R32_UINT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA } })
{
    createHUDTextures(device, context);
    setHUDTextRenderPos();
    createHUDVBS(device);
    createHUDCBs(device);
    
   sFont[0] = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/comicsans.spritefont");
   sBatch = std::make_unique<DirectX::SpriteBatch>(context);

   changed = false;
   firstTime = true;
   prevHP = 3;
   prevCooldown = 1.0f;
}

Graphics::HUD::~HUD()
{
    SAFE_RELEASE(vertexBuffer);
    SAFE_RELEASE(HUDCBuffer);
    SAFE_RELEASE(hudTextures[0]);
    SAFE_RELEASE(hudTextures[1]);
    SAFE_RELEASE(hudTextures[2]);
    SAFE_RELEASE(hudTextures[3]);
    SAFE_RELEASE(hudTextures[4]);
}

void Graphics::HUD::drawHUD(ID3D11DeviceContext * context, ID3D11RenderTargetView * backBuffer, ID3D11BlendState * blendState)
{

    if (changed)
    {
        updateHUDConstantBuffer(context);
    }
    
    UINT stride = 20, offset = 0;
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(shader);

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT sampleMask = 0xffffffff;
    context->OMSetBlendState(blendState, blendFactor, sampleMask);
 
    context->OMSetRenderTargets(1, &backBuffer, nullptr);

    context->VSSetConstantBuffers(0,1, &HUDCBuffer);
    context->VSSetShader(shader, nullptr, 0);

    context->PSSetShaderResources(0, 5, hudTextures);
    context->PSSetShader(shader, nullptr, 0);

    context->Draw(24, 0);
    ID3D11ShaderResourceView * SRVNULL = nullptr;
    context->PSSetShaderResources(0, 1, &SRVNULL);

    renderText(blendState);
}

void Graphics::HUD::queueText(Graphics::TextString * text)
{
    textQueue.push_back(*text);
}

//gives the graphics side the info to render the hud
void Graphics::HUD::fillHUDInfo(HUDInfo * info)
{
    if (!firstTime)
    {
        if (prevHP != info->hp || (prevCooldown - info->cd) > 0.001f)
        {
            changed = true;
            prevHP = info->hp;
            prevCooldown = info->cd;
        }
    }
    else
    {
        currentInfo = info;
        firstTime = false;
    }
    
}

void Graphics::HUD::createHUDVBS(ID3D11Device * device)
{
    struct GUI
    {
        DirectX::SimpleMath::Vector2 verts;
        DirectX::SimpleMath::Vector2 uv;
        UINT element;
    };

    GUI GUIquad[24];
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


    GUIquad[12].verts = DirectX::SimpleMath::Vector2{ -1.0f, -1.0f };
    GUIquad[12].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

    GUIquad[13].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.8f };
    GUIquad[13].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[14].verts = DirectX::SimpleMath::Vector2{ -0.8f, -1.0f };
    GUIquad[14].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[15].verts = DirectX::SimpleMath::Vector2{ -0.8f, -0.8f };
    GUIquad[15].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

    GUIquad[16].verts = GUIquad[14].verts;
    GUIquad[16].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[17].verts = GUIquad[13].verts;
    GUIquad[17].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[12].element = 2;
    GUIquad[13].element = 2;
    GUIquad[14].element = 2;
    GUIquad[15].element = 2;
    GUIquad[16].element = 2;
    GUIquad[17].element = 2;

    GUIquad[18].verts = DirectX::SimpleMath::Vector2{ 2 * (float)((cdPos.x - 31 + 29) / WIN_WIDTH )- 1,  2 * (float)((WIN_HEIGHT - cdPos.y - 31 -14) / WIN_HEIGHT) - 1 };
    GUIquad[18].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

    GUIquad[19].verts = DirectX::SimpleMath::Vector2{ 2 * (float)((cdPos.x - 31 + 29 ) / WIN_WIDTH) - 1,  2 * (float)((WIN_HEIGHT - cdPos.y + 31 -14) / WIN_HEIGHT) - 1 };
    GUIquad[19].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[20].verts = DirectX::SimpleMath::Vector2{ 2 * (float)((cdPos.x + 31 + 29) / WIN_WIDTH) - 1,  2 * (float)((WIN_HEIGHT - cdPos.y - 31 -14) / WIN_HEIGHT) - 1 };
    GUIquad[20].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[21].verts = DirectX::SimpleMath::Vector2{ 2 * (float)((cdPos.x + 31 + 29) / WIN_WIDTH) - 1,   2 * (float)((WIN_HEIGHT -cdPos.y + 31 -14) / WIN_HEIGHT) - 1 };
    GUIquad[21].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

    GUIquad[22].verts = GUIquad[20].verts;
    GUIquad[22].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[23].verts = GUIquad[19].verts;
    GUIquad[23].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[18].element = 3;
    GUIquad[19].element = 3;
    GUIquad[20].element = 3;
    GUIquad[21].element = 3;
    GUIquad[22].element = 3;
    GUIquad[23].element = 3;

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

    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("crosshair.png"), nullptr, &hudTextures[0]));
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("HPbar.png"), nullptr, &hudTextures[1]));
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("HPbarOutline.png"), nullptr, &hudTextures[2]));
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("cooldownCircle.png"), nullptr, &hudTextures[3]));
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("cooldownCircleActive.png"), nullptr, &hudTextures[4]));

  
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
    ammoPos1 = DirectX::SimpleMath::Vector2((WIN_WIDTH / 2)  + 60 , (WIN_HEIGHT / 2) + 50);
    ammoPos2 = DirectX::SimpleMath::Vector2((WIN_WIDTH - 250) , (WIN_HEIGHT / 2) + 300);

    cdPos = DirectX::SimpleMath::Vector2((WIN_WIDTH / 2) - 24, (WIN_HEIGHT / 2) + 120);
    
    scorePos = DirectX::SimpleMath::Vector2(0, 0);
    wavePos = DirectX::SimpleMath::Vector2(0, 0);
    timePos = DirectX::SimpleMath::Vector2(0, 0);
}

void Graphics::HUD::renderHUDText()
{   
    std::wstring temp = L"";
    if (!currentInfo->sledge)
    {
        temp = std::to_wstring(currentInfo->cuttleryAmmo[0]);
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
    int tempInt = (currentInfo->cd)* 100;
    temp = (std::to_wstring(tempInt));
    temp += L"%";
    sFont[0]->DrawString(sBatch.get(), temp.c_str(), cdPos, DirectX::Colors::Red);
}

//updates the hp and cooldown values on the GPU side
void Graphics::HUD::updateHUDConstantBuffer(ID3D11DeviceContext * context)
{
    float temp[2] = { 1.0f };
    //3.0f comes from the max HP of the player
    temp[0] = 1.0f - (float)(currentInfo->hp / 3.0f);
    temp[1] = currentInfo->cd;
    //temp[0] =  1.0f - (float)(1 / 3.0f);

    D3D11_MAPPED_SUBRESOURCE data = { 0 };
    ThrowIfFailed(context->Map(HUDCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data));
    memcpy(data.pData, temp, sizeof(float) * 2);

    context->Unmap(HUDCBuffer, 0);

    changed = false;
}

void Graphics::HUD::createHUDCBs(ID3D11Device * device)
{
    D3D11_BUFFER_DESC desc = { 0 };
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.ByteWidth = sizeof(float) * 4;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    
    float temp[2] = { 0.0f, 0.0f };
    D3D11_SUBRESOURCE_DATA data = { 0 };
    data.pSysMem = temp;

    ThrowIfFailed(device->CreateBuffer(&desc, &data, &HUDCBuffer));

}

