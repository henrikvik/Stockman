#include "HUD.H"
#include <WICTextureLoader.h>
#include <Logic\include\Misc\RandomGenerator.h>

#include <Engine\Profiler.h>

Graphics::HUD::HUD(ID3D11Device * device, ID3D11DeviceContext * context)
	:shader(device, SHADER_PATH("GUIShader.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } ,{ "ELEMENT", 0, DXGI_FORMAT_R32_UINT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA } }),
	offsetBuffer(device)
{
    createHUDTextures(device, context);
    setHUDTextRenderPos();
    createHUDVBS(device);
    createHUDCBs(device);

	offset.x = 0;
	offset.y = 0;
	offsetBuffer.write(context, &offset, sizeof(DirectX::SimpleMath::Vector2));
    
   sFont[0] = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/comicsans.spritefont");
   sBatch = std::make_unique<DirectX::SpriteBatch>(context);

   changed = false;
   firstTime = true;
   prevHP = 3;
   prevCooldown = 1.0f;
   isShaking = false;
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
    
    static UINT stride = 20, offset = 0;
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(shader);

    static float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static UINT sampleMask = 0xffffffff;
    context->OMSetBlendState(blendState, blendFactor, sampleMask);
 
    context->OMSetRenderTargets(1, &backBuffer, nullptr);

    context->VSSetConstantBuffers(0, 1, &HUDCBuffer);
	context->VSSetConstantBuffers(1, 1, offsetBuffer);
    context->VSSetShader(shader, nullptr, 0);

    context->PSSetShaderResources(0, 5, hudTextures);
    context->PSSetShader(shader, nullptr, 0);

    context->Draw(24, 0);
    ID3D11ShaderResourceView * SRVNULL = nullptr;
    context->PSSetShaderResources(0, 1, &SRVNULL);

    PROFILE_BEGIN("draw text");
    renderText(blendState);
    //renderHUDText(blendState);
    PROFILE_END();
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

void Graphics::HUD::startShake(float radius, float duration)
{
	isShaking = true;
	shakeRadius = radius;
	shakeDuration = duration;
	shakeCounter = 0;
}

void Graphics::HUD::updateShake(ID3D11DeviceContext * context, float deltaTime)
{
	if (isShaking)
	{
		float currentShakeAmount = shakeRadius * (1 - (shakeCounter / shakeDuration));

		static float angle = 0;
		angle = Logic::RandomGenerator::singleton().getRandomFloat(0, 360);
		offset = DirectX::SimpleMath::Vector2(sin(angle) * currentShakeAmount, cos(angle) * currentShakeAmount);

		

		shakeCounter += deltaTime;
		if (shakeCounter >= shakeDuration)
		{
			isShaking = false;
			offset = DirectX::SimpleMath::Vector2(0, 0);
		}

		DirectX::SimpleMath::Vector2 NDCOffset((float)(offset.x * 2.f / WIN_WIDTH), ((WIN_HEIGHT - offset.y) / WIN_HEIGHT) - 1.f);

		offsetBuffer.write(context, &NDCOffset, sizeof(NDCOffset));
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

    //weapon icon 1

    GUIquad[18].verts = DirectX::SimpleMath::Vector2{ -1.0f, -1.0f };
    GUIquad[18].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

    GUIquad[19].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.8f };
    GUIquad[19].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[20].verts = DirectX::SimpleMath::Vector2{ -0.8f, -1.0f };
    GUIquad[20].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[21].verts = DirectX::SimpleMath::Vector2{ -0.8f, -0.8f };
    GUIquad[21].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

    GUIquad[22].verts = GUIquad[14].verts;
    GUIquad[22].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[23].verts = GUIquad[13].verts;
    GUIquad[23].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[18].element = 3;
    GUIquad[19].element = 3;
    GUIquad[20].element = 3;
    GUIquad[21].element = 3;
    GUIquad[22].element = 3;
    GUIquad[23].element = 3;

    //weapon icon 2

    GUIquad[24].verts = DirectX::SimpleMath::Vector2{ -1.0f, -1.0f };
    GUIquad[24].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

    GUIquad[25].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.8f };
    GUIquad[25].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[26].verts = DirectX::SimpleMath::Vector2{ -0.8f, -1.0f };
    GUIquad[26].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[27].verts = DirectX::SimpleMath::Vector2{ -0.8f, -0.8f };
    GUIquad[27].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

    GUIquad[28].verts = GUIquad[14].verts;
    GUIquad[28].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[29].verts = GUIquad[13].verts;
    GUIquad[29].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[24].element = 4;
    GUIquad[25].element = 4;
    GUIquad[26].element = 4;
    GUIquad[27].element = 4;
    GUIquad[28].element = 4;
    GUIquad[29].element = 4;

    //skill icon 1

    GUIquad[30].verts = DirectX::SimpleMath::Vector2{ -1.0f, -1.0f };
    GUIquad[30].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

    GUIquad[31].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.8f };
    GUIquad[31].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[32].verts = DirectX::SimpleMath::Vector2{ -0.8f, -1.0f };
    GUIquad[32].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[33].verts = DirectX::SimpleMath::Vector2{ -0.8f, -0.8f };
    GUIquad[33].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

    GUIquad[34].verts = GUIquad[14].verts;
    GUIquad[34].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[35].verts = GUIquad[13].verts;
    GUIquad[35].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[30].element = 5;
    GUIquad[31].element = 5;
    GUIquad[32].element = 5;
    GUIquad[33].element = 5;
    GUIquad[34].element = 5;
    GUIquad[35].element = 5;

    //weapon icon 2

    GUIquad[36].verts = DirectX::SimpleMath::Vector2{ -1.0f, -1.0f };
    GUIquad[36].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

    GUIquad[37].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.8f };
    GUIquad[37].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[38].verts = DirectX::SimpleMath::Vector2{ -0.8f, -1.0f };
    GUIquad[38].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[39].verts = DirectX::SimpleMath::Vector2{ -0.8f, -0.8f };
    GUIquad[39].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

    GUIquad[40].verts = GUIquad[14].verts;
    GUIquad[40].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[41].verts = GUIquad[13].verts;
    GUIquad[41].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[36].element = 6;
    GUIquad[37].element = 6;
    GUIquad[38].element = 6;
    GUIquad[39].element = 6;
    GUIquad[40].element = 6;
    GUIquad[41].element = 6;

   /* GUIquad[18].verts = DirectX::SimpleMath::Vector2{ 2 * (float)((cdPos.x - 31 + 29) / WIN_WIDTH )- 1,  2 * (float)((WIN_HEIGHT - cdPos.y - 31 -14) / WIN_HEIGHT) - 1 };
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
    GUIquad[23].element = 3;*/

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

    if (!firstTime)
    {
        renderHUDText(blendState);
    }
    
    sBatch->End();
}

void Graphics::HUD::setHUDTextRenderPos()
{
    ammoPos1 = DirectX::SimpleMath::Vector2((WIN_WIDTH / 2)  + 60 , (WIN_HEIGHT / 2) + 50);
    ammoPos2 = DirectX::SimpleMath::Vector2((WIN_WIDTH - 250) , (WIN_HEIGHT / 2) + 300);

    cdPos = DirectX::SimpleMath::Vector2((WIN_WIDTH / 2) - 24, (WIN_HEIGHT / 2) + 120);
    
    scorePos = DirectX::SimpleMath::Vector2((WIN_WIDTH / 2 - 24), (WIN_HEIGHT / 2) - 300);
    wavePos = DirectX::SimpleMath::Vector2(0, 0);
    timePos = DirectX::SimpleMath::Vector2(0, 0);
}

void Graphics::HUD::renderHUDText(ID3D11BlendState * blendState)
{   
    //sBatch->Begin(DirectX::SpriteSortMode_Deferred, blendState);
    std::wstring temp = L"";
    if (!currentInfo->sledge)
    {
        temp = std::to_wstring(currentInfo->cuttleryAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->cuttleryAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos1 + offset, DirectX::Colors::Red);

        temp = std::to_wstring(currentInfo->iceAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->iceAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos2 + offset, DirectX::Colors::Red);
    }
    else
    {
        std::wstring temp = std::to_wstring(currentInfo->cuttleryAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->cuttleryAmmo[1]);

        DirectX::SimpleMath::Vector2 tempPos = ammoPos2 + offset;
        tempPos.y += 20;
        sFont[0]->DrawString(sBatch.get(), temp.c_str(), tempPos + offset, DirectX::Colors::Red);

        temp = std::to_wstring(currentInfo->iceAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->iceAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos2 + offset, DirectX::Colors::Red);
    }
    int tempInt = (1 - currentInfo->cd)* 100;
    if (tempInt == 0)
    {
        tempInt = 100;
    }
    temp = (std::to_wstring(tempInt));
    temp += L"%";
    sFont[0]->DrawString(sBatch.get(), temp.c_str(), cdPos + offset, DirectX::Colors::Red);

	temp = (std::to_wstring(currentInfo->score));
	temp += L" Points";
	sFont[0]->DrawString(sBatch.get(), temp.c_str(), scorePos + offset, DirectX::Colors::White);
	//sFont[0]->DrawString(sBatch.get(), temp.c_str(), scorePos, DirectX::Colors::White);
    //sBatch->End();
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
    desc.ByteWidth = 32;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    
    float temp[8] = { 0.0f};
    D3D11_SUBRESOURCE_DATA data = { 0 };
    data.pSysMem = temp;

    ThrowIfFailed(device->CreateBuffer(&desc, &data, &HUDCBuffer));

}

