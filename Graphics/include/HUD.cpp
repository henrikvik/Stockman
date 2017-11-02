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
    
   sFont[0] = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/KGshe.spritefont");
   sFont[1] = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/KGshelarger.spritefont");
   sFont[2] = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/KGshelargest.spritefont");
   sBatch = std::make_unique<DirectX::SpriteBatch>(context);

   changed = false;
   firstTime = true;
   prevHP = 3;
   prevCooldown0 = 1.0f;
   prevCooldown1 = 1.0f;
   isShaking = false;
   prevWeapon = 0;
   vbCreated = false;
   fuligasehacket = false;
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


    if (!vbCreated && !firstTime && currentInfo->currentSkills[0] >= 0)
    {
        this->setSkillIcons(context);
        vbCreated = true;
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

    context->PSSetShaderResources(0, 4, hudTextures);
    context->PSSetShader(shader, nullptr, 0);

    context->Draw(48, 0);
    ID3D11ShaderResourceView * SRVNULL = nullptr;
    context->PSSetShaderResources(0, 1, &SRVNULL);

    PROFILE_BEGIN("draw text");
    renderText(blendState, true);
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
        if (prevHP != info->hp || (prevCooldown0 - info->cd0) > 0.001f || (prevCooldown1 - info->cd1) > 0.001f || prevWeapon != info->currentWeapon)
        {
            changed = true;
            prevHP = info->hp;
            prevWeapon = info->activeAmmo[2];
            prevCooldown0 = info->cd0;
        }
    }
    else
    {
        currentInfo = info;
        firstTime = false;
        changed = true;
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

    GUI GUIquad[48] = {};
   

    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.ByteWidth = sizeof(GUIquad);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = GUIquad;


    ThrowIfFailed(device->CreateBuffer(&desc, &data, &vertexBuffer));
}

void Graphics::HUD::createHUDTextures(ID3D11Device * device, ID3D11DeviceContext * context)
{

    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("crosshair.png"), nullptr, &hudTextures[0]));
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("HPBars.dds"), nullptr, &hudTextures[1]));
    //ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("HPbarOutline.png"), nullptr, &hudTextures[2]));
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("HUDIcons.dds"), nullptr, &hudTextures[3]));
    //ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("cooldownCircleActive.png"), nullptr, &hudTextures[4]));

  
}

void Graphics::HUD::renderText(ID3D11BlendState * blendState, bool fulhack)
{
    sBatch->Begin(DirectX::SpriteSortMode_Deferred, blendState);

    for (size_t i = 0; i < textQueue.size(); i++)
    {
        TextString temp = textQueue.at(i);

            sFont[temp.font]->DrawString(sBatch.get(), temp.text.c_str(), temp.pos, temp.color);   
    }
    textQueue.clear();

    if (fulhack)
    {
        renderHUDText(blendState);
    }
    
    sBatch->End();
}

void Graphics::HUD::setHUDTextRenderPos()
{
    ammoPos1 = DirectX::SimpleMath::Vector2((WIN_WIDTH - 450), (WIN_HEIGHT / 2) + 318);
    ammoPos2 = DirectX::SimpleMath::Vector2((WIN_WIDTH - 370) , (WIN_HEIGHT / 2) + 318);

   

    cdPos0 = DirectX::SimpleMath::Vector2((WIN_WIDTH - 170), (WIN_HEIGHT / 2) + 315);
    cdPos1 = DirectX::SimpleMath::Vector2((WIN_WIDTH - 70), (WIN_HEIGHT / 2) + 315);
   
    
    scorePos = DirectX::SimpleMath::Vector2((WIN_WIDTH / 2 - 24), (WIN_HEIGHT / 2) - 300);
    wavePos = DirectX::SimpleMath::Vector2(0, 0);
    timePos = DirectX::SimpleMath::Vector2(0, 0);
}

void Graphics::HUD::renderHUDText(ID3D11BlendState * blendState)
{   
    //sBatch->Begin(DirectX::SpriteSortMode_Deferred, blendState);
    std::wstring temp = L"";
    if (currentInfo->currentWeapon == 0)
    {
        temp = std::to_wstring(currentInfo->activeAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->activeAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos1 + offset, DirectX::Colors::Red);

        temp = std::to_wstring(currentInfo->inactiveAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->inactiveAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos2 + offset, DirectX::Colors::Red);
    }
    else if (currentInfo->currentWeapon == 1)
    {
        

        temp = std::to_wstring(currentInfo->activeAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->activeAmmo[1]);


        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos2 + offset, DirectX::Colors::Red);

        temp = std::to_wstring(currentInfo->inactiveAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->inactiveAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos1 + offset, DirectX::Colors::Red);

        
    }
    else
    {
        temp = std::to_wstring(currentInfo->activeAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->activeAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos2 + offset, DirectX::Colors::Red);

        temp = std::to_wstring(currentInfo->inactiveAmmo[0]);
        temp += L"/";
        temp += std::to_wstring(currentInfo->inactiveAmmo[1]);

        sFont[0]->DrawString(sBatch.get(), temp.c_str(), ammoPos1 + offset, DirectX::Colors::Red);
    }




    int tempInt = (1 - currentInfo->cd0)* 100;
    if (tempInt == 0)
    {
        tempInt = 100;
    }
    temp = (std::to_wstring(tempInt));
    temp += L"%";
    sFont[0]->DrawString(sBatch.get(), temp.c_str(), cdPos0 + offset, DirectX::Colors::Red);

    tempInt = (1 - currentInfo->cd1) * 100;
    if (tempInt == 0)
    {
        tempInt = 100;
    }
    temp = (std::to_wstring(tempInt));
    temp += L"%";
    sFont[0]->DrawString(sBatch.get(), temp.c_str(), cdPos1 + offset, DirectX::Colors::Red);

	temp = (std::to_wstring(currentInfo->score));
	temp += L"p";
	sFont[0]->DrawString(sBatch.get(), temp.c_str(), scorePos + offset, DirectX::Colors::White);
}

//updates the hp and cooldown values on the GPU side
void Graphics::HUD::updateHUDConstantBuffer(ID3D11DeviceContext * context)
{
    float temp[6] = { 0.0f };
    //3.0f comes from the max HP of the player
    temp[0] = 1.0f - (float)(currentInfo->hp / 3.0f);


    if (currentInfo->currentWeapon == 0)
    {
        temp[1] = 0.5f;
        temp[2] = 0.0f;
        temp[3] = 0.0f;
    }
    else if (currentInfo->currentWeapon == 1)
    {
        temp[1] = 0.0f;
        temp[2] = 0.5f;
        temp[3] = 0.0f;
    }
    else
    {
        temp[1] = 0.0f;
        temp[2] = 0.0f;
        temp[3] = 0.5f;
    }


    if ((1 -currentInfo->cd0) < 0.001f)
    {
        temp[4] = 0.5f;
    }
    else
    {
        temp[4] = 0.0f;
    }

    if ((1 - currentInfo->cd1) < 0.001f)
    {
        temp[5] = 0.5f;
    }
    else
    {
        temp[5] = 0.0f;
    }




    D3D11_MAPPED_SUBRESOURCE data = { 0 };
    ThrowIfFailed(context->Map(HUDCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data));
    memcpy(data.pData, temp, sizeof(float) * 6);

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

void Graphics::HUD::setSkillIcons(ID3D11DeviceContext *context)
{
    struct GUI
    {
        DirectX::SimpleMath::Vector2 verts;
        DirectX::SimpleMath::Vector2 uv;
        UINT element;
    };

    GUI GUIquad[48];
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

    GUIquad[6].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.9f };
    GUIquad[6].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.35f };

    GUIquad[7].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.7f };
    GUIquad[7].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.f };

    GUIquad[8].verts = DirectX::SimpleMath::Vector2{ -0.65f, -0.9f };
    GUIquad[8].uv = DirectX::SimpleMath::Vector2{ 1.0f, .35f };

    GUIquad[9].verts = DirectX::SimpleMath::Vector2{ -0.65f, -0.7f };
    GUIquad[9].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

    GUIquad[10].verts = GUIquad[8].verts;
    GUIquad[10].uv = DirectX::SimpleMath::Vector2{ 1.0f, .35f };

    GUIquad[11].verts = GUIquad[7].verts;
    GUIquad[11].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

    GUIquad[6].element = 1;
    GUIquad[7].element = 1;
    GUIquad[8].element = 1;
    GUIquad[9].element = 1;
    GUIquad[10].element = 1;
    GUIquad[11].element = 1;


    GUIquad[12].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.9f };
    GUIquad[12].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

    GUIquad[13].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.7f };
    GUIquad[13].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.5f };

    GUIquad[14].verts = DirectX::SimpleMath::Vector2{ -0.65f, -0.9f };
    GUIquad[14].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.f };

    GUIquad[15].verts = DirectX::SimpleMath::Vector2{ -0.65f, -0.7f };
    GUIquad[15].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.5f };

    GUIquad[16].verts = GUIquad[14].verts;
    GUIquad[16].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

    GUIquad[17].verts = GUIquad[13].verts;
    GUIquad[17].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.5f };

    GUIquad[12].element = 2;
    GUIquad[13].element = 2;
    GUIquad[14].element = 2;
    GUIquad[15].element = 2;
    GUIquad[16].element = 2;
    GUIquad[17].element = 2;

    //weapon icon 1

    GUIquad[18].verts = DirectX::SimpleMath::Vector2{ 0.3f, -.9f };
    GUIquad[18].uv = DirectX::SimpleMath::Vector2{ 1.f / 6.1f, 0.5f };

    GUIquad[19].verts = DirectX::SimpleMath::Vector2{ 0.3f, -0.65f };
    GUIquad[19].uv = DirectX::SimpleMath::Vector2{ 1.f / 6.1f, 0.0f };

    GUIquad[20].verts = DirectX::SimpleMath::Vector2{ 0.42f, -.9f };
    GUIquad[20].uv = DirectX::SimpleMath::Vector2{ 2.f / 6.5f, 0.50f };

    GUIquad[21].verts = DirectX::SimpleMath::Vector2{ 0.42f, -0.65f };
    GUIquad[21].uv = DirectX::SimpleMath::Vector2{ 2.f / 6.5f, 0.0f };

    GUIquad[22].verts = GUIquad[20].verts;
    GUIquad[22].uv = DirectX::SimpleMath::Vector2{ 2.f / 6.5f, 0.5f };

    GUIquad[23].verts = GUIquad[19].verts;
    GUIquad[23].uv = DirectX::SimpleMath::Vector2{ 1.f / 6.1f, 0.0f };

    GUIquad[18].element = 3;
    GUIquad[19].element = 3;
    GUIquad[20].element = 3;
    GUIquad[21].element = 3;
    GUIquad[22].element = 3;
    GUIquad[23].element = 3;

    //weapon icon 2

    GUIquad[24].verts = DirectX::SimpleMath::Vector2{ 0.42f, -.9f };
    GUIquad[24].uv = DirectX::SimpleMath::Vector2{ 0.f, 0.5f };

    GUIquad[25].verts = DirectX::SimpleMath::Vector2{ 0.42f, -0.65f };
    GUIquad[25].uv = DirectX::SimpleMath::Vector2{ 0.f, 0.0f };

    GUIquad[26].verts = DirectX::SimpleMath::Vector2{ 0.55f, -.9f };
    GUIquad[26].uv = DirectX::SimpleMath::Vector2{ 1.f / 6.15f, 0.50f };

    GUIquad[27].verts = DirectX::SimpleMath::Vector2{ 0.55f, -0.65f };
    GUIquad[27].uv = DirectX::SimpleMath::Vector2{ 1.f / 6.15f, 0.0f };

    GUIquad[28].verts = GUIquad[26].verts;
    GUIquad[28].uv = DirectX::SimpleMath::Vector2{ 1.f / 6.15f, 0.5f };

    GUIquad[29].verts = GUIquad[25].verts;
    GUIquad[29].uv = DirectX::SimpleMath::Vector2{ 0.f, 0.0f };

    GUIquad[24].element = 4;
    GUIquad[25].element = 4;
    GUIquad[26].element = 4;
    GUIquad[27].element = 4;
    GUIquad[28].element = 4;
    GUIquad[29].element = 4;

    //weapon icon 3

    GUIquad[30].verts = DirectX::SimpleMath::Vector2{ 0.56f, -.9f };
    GUIquad[30].uv = DirectX::SimpleMath::Vector2{ 2.f / 6.5f, 0.50f };

    GUIquad[31].verts = DirectX::SimpleMath::Vector2{ 0.56f, -0.65f };
    GUIquad[31].uv = DirectX::SimpleMath::Vector2{ 2.f / 6.5f, 0.0f };

    GUIquad[32].verts = DirectX::SimpleMath::Vector2{ 0.71f, -.9f };
    GUIquad[32].uv = DirectX::SimpleMath::Vector2{ 3.f / 6.2f, 0.5f };

    GUIquad[33].verts = DirectX::SimpleMath::Vector2{ 0.71f, -0.65f };
    GUIquad[33].uv = DirectX::SimpleMath::Vector2{ 3.f / 6.2f, 0.0f };

    GUIquad[34].verts = GUIquad[32].verts;
    GUIquad[34].uv = DirectX::SimpleMath::Vector2{ 3.f / 6.2f, 0.5f };

    GUIquad[35].verts = GUIquad[31].verts;
    GUIquad[35].uv = DirectX::SimpleMath::Vector2{ 2.f / 6.5f, 0.0f };

    GUIquad[30].element = 5;
    GUIquad[31].element = 5;
    GUIquad[32].element = 5;
    GUIquad[33].element = 5;
    GUIquad[34].element = 5;
    GUIquad[35].element = 5;

    //skill icon 1

    float skillUV0 = 3.f / 6.2f + (1.0f/6.0f * currentInfo->currentSkills[1]);
    float skillUV1 = 3.f / 6.1f + (1.0f / 6.0f * (currentInfo->currentSkills[1] +1 )) ;

    GUIquad[36].verts = DirectX::SimpleMath::Vector2{ 0.7f, -.9f };
    GUIquad[36].uv = DirectX::SimpleMath::Vector2{ skillUV0, 0.5f };

    GUIquad[37].verts = DirectX::SimpleMath::Vector2{ 0.7f, -0.65f };
    GUIquad[37].uv = DirectX::SimpleMath::Vector2{ skillUV0, 0.0f };

    GUIquad[38].verts = DirectX::SimpleMath::Vector2{ 0.85f, -.9f };
    GUIquad[38].uv = DirectX::SimpleMath::Vector2{ skillUV1, 0.5f };

    GUIquad[39].verts = DirectX::SimpleMath::Vector2{ 0.85f, -0.65f };
    GUIquad[39].uv = DirectX::SimpleMath::Vector2{ skillUV1, 0.0f };

    GUIquad[40].verts = GUIquad[38].verts;
    GUIquad[40].uv = DirectX::SimpleMath::Vector2{ skillUV1, 0.5f };

    GUIquad[41].verts = GUIquad[37].verts;
    GUIquad[41].uv = DirectX::SimpleMath::Vector2{ skillUV0, 0.0f };

    GUIquad[36].element = 6;
    GUIquad[37].element = 6;
    GUIquad[38].element = 6;
    GUIquad[39].element = 6;
    GUIquad[40].element = 6;
    GUIquad[41].element = 6;

    //skill icon 2

    skillUV0 = 3.f / 6.2f + (1.0f / 6.0f) * currentInfo->currentSkills[0];
    skillUV1 = 3.f / 6.1f + (1.0f / 6.0f) * (currentInfo->currentSkills[0] + 1);

    GUIquad[42].verts = DirectX::SimpleMath::Vector2{ 0.85f, -.9f };
    GUIquad[42].uv = DirectX::SimpleMath::Vector2{ skillUV0, 0.5f };

    GUIquad[43].verts = DirectX::SimpleMath::Vector2{ 0.85f, -0.65f };
    GUIquad[43].uv = DirectX::SimpleMath::Vector2{ skillUV0, 0.0f };

    GUIquad[44].verts = DirectX::SimpleMath::Vector2{ 1.0f, -.9f };
    GUIquad[44].uv = DirectX::SimpleMath::Vector2{ skillUV1, 0.5f };

    GUIquad[45].verts = DirectX::SimpleMath::Vector2{ 1.0f, -0.65f };
    GUIquad[45].uv = DirectX::SimpleMath::Vector2{ skillUV1, 0.0f };

    GUIquad[46].verts = GUIquad[44].verts;
    GUIquad[46].uv = DirectX::SimpleMath::Vector2{ skillUV1, 0.5f };

    GUIquad[47].verts = GUIquad[43].verts;
    GUIquad[47].uv = DirectX::SimpleMath::Vector2{ skillUV0, 0.0f };

    GUIquad[42].element = 7;
    GUIquad[43].element = 7;
    GUIquad[44].element = 7;
    GUIquad[45].element = 7;
    GUIquad[46].element = 7;
    GUIquad[47].element = 7;

    D3D11_MAPPED_SUBRESOURCE data = { 0 };
    ThrowIfFailed(context->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data));

    memcpy(data.pData, GUIquad, sizeof(GUI) * 48);

    context->Unmap(this->vertexBuffer, 0);



}

