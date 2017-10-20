#include "Menu.h"
#include <WICTextureLoader.h>

Graphics::Menu::Menu(ID3D11Device * device, ID3D11DeviceContext * contex)
    : shader(device, SHADER_PATH("MenuShader.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA } })
{
    this->active = nullptr;
    this->loaded = false;
    this->states = new DirectX::CommonStates(device);

    createVBuffers(device);
}
Graphics::Menu::~Menu()
{
    
    SAFE_RELEASE(buttonTexture[0]);
    SAFE_RELEASE(buttonTexture[1]);
    SAFE_RELEASE(buttonTexture[2]);
    SAFE_RELEASE(menuTexture[0]);
    SAFE_RELEASE(menuTexture[1]);
    SAFE_RELEASE(menuTexture[2]);
    SAFE_RELEASE(menuTexture[3]);
    SAFE_RELEASE(menuQuad);
    SAFE_RELEASE(buttonQuad);
    delete states;
}

void Graphics::Menu::drawMenu(ID3D11Device * device, ID3D11DeviceContext * contex, Graphics::MenuInfo * info, ID3D11RenderTargetView * backBuffer, ID3D11BlendState * blendState)
{
    active = info;
    loadTextures(device, contex);
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    contex->ClearRenderTargetView(backBuffer, clearColor);

    UINT stride = sizeof(Graphics::TriangleVertex), offset = 0;
    contex->IASetVertexBuffers(0, 1, &menuQuad, &stride, &offset);

    contex->IASetInputLayout(shader);
    contex->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    contex->VSSetShader(shader, nullptr, 0);
    contex->PSSetShader(shader, nullptr, 0);
    auto sampler = states->PointWrap();
    contex->PSSetSamplers(0, 1, &sampler);
    contex->PSSetShaderResources(0, 1, &menuTexture[active->m_menuTexture]);

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT sampleMask = 0xffffffff;
    contex->OMSetBlendState(blendState, blendFactor, sampleMask);
    contex->OMSetRenderTargets(1, &backBuffer, nullptr);

    contex->Draw(6, 0);

    contex->PSSetShaderResources(0, 1, &buttonTexture[active->m_buttons.at(0).textureIndex]);
    for (size_t i = 0; i < info->m_buttons.size(); i++)
    {
        mapButtons(contex, &info->m_buttons.at(i));
        contex->IASetVertexBuffers(0, 1, &buttonQuad, &stride, &offset);
        contex->Draw(6, 0);
    }
}

void Graphics::Menu::loadTextures(ID3D11Device * device, ID3D11DeviceContext * context)
{
    if (loaded == false)
    {

        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("mainMenuTexture.png"), nullptr, &menuTexture[0]));
        menuTexture[1] = nullptr;
        menuTexture[2] = nullptr;

        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("SettingsMenuTexture.png"), nullptr, &menuTexture[1]));
        //ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("mainMenuTexture.png"), nullptr, &menuTexture[2]));

        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("gameOverTexture.png"), nullptr, &menuTexture[3]));

        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("mainMenuButton.png"), nullptr, &buttonTexture[0]));
        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("gameOverMenuButtons.png"), nullptr, &buttonTexture[1]));
        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, context, TEXTURE_PATH("SettingsMenuButtons.png"), nullptr, &buttonTexture[2]));
        loaded = true;
    }
}

void Graphics::Menu::unloadTextures()
{
    if (loaded == true)
    {
        SAFE_RELEASE(buttonTexture[0]);
        SAFE_RELEASE(buttonTexture[1]);
        SAFE_RELEASE(buttonTexture[2]);
        SAFE_RELEASE(menuTexture[0]);
        SAFE_RELEASE(menuTexture[1]);
        //SAFE_RELEASE(menuTexture[2]);
        SAFE_RELEASE(menuTexture[3]);
        loaded = false;
    }
}

//maps the button VB to the button past
void Graphics::Menu::mapButtons(ID3D11DeviceContext * contex, ButtonInfo * info)
{

    //moves the buttons to ndc space
    TriangleVertex triangleVertices[6] =
    {
        2 * ((float)((info->m_rek.x + info->m_rek.width)) / WIN_WIDTH) - 1, 2 * ((float)(WIN_HEIGHT - info->m_rek.y - info->m_rek.height) / WIN_HEIGHT) - 1, 0.0f,	//v0 pos
        info->m_texCoordEnd.x, info->m_texCoordEnd.y,

        2 * ((float)(info->m_rek.x) / WIN_WIDTH) - 1 , 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height)) / WIN_HEIGHT) - 1, 0.0f,	//v1
        info->m_texCoordStart.x, info->m_texCoordEnd.y,

        2 * ((float)(info->m_rek.x) / WIN_WIDTH) - 1 , 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height + info->m_rek.height)) / WIN_HEIGHT) - 1, 0.0f, //v2
        info->m_texCoordStart.x,  info->m_texCoordStart.y,

        //t2
        2 * ((float)(info->m_rek.x) / WIN_WIDTH) - 1 , 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height + info->m_rek.height)) / WIN_HEIGHT) - 1, 0.0f,	//v2 pos
        info->m_texCoordStart.x, info->m_texCoordStart.y,

        2 * ((float)((info->m_rek.x + info->m_rek.width)) / WIN_WIDTH) - 1, 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height + info->m_rek.height)) / WIN_HEIGHT) - 1 , 0.0f,	//v3
        info->m_texCoordEnd.x, info->m_texCoordStart.y,

        2 * ((float)((info->m_rek.x + info->m_rek.width)) / WIN_WIDTH) - 1, 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height)) / WIN_HEIGHT) - 1 , 0.0f, //v0
        info->m_texCoordEnd.x, info->m_texCoordEnd.y,
    };

    D3D11_MAPPED_SUBRESOURCE data = { 0 };
    ThrowIfFailed(contex->Map(buttonQuad, 0, D3D11_MAP_WRITE_DISCARD, 0, &data));
    memcpy(data.pData, triangleVertices, sizeof(TriangleVertex) * 6);
    contex->Unmap(buttonQuad, 0);

}

//creates the vertexbuffers the menu uses.
void Graphics::Menu::createVBuffers(ID3D11Device * device)
{
    //menu fullscreen quad

    TriangleVertex triangleVertices[6] =
    {
        1.f, -1.f, 0.0f,	//v0 pos
        1.0f, 1.0f,

        -1.f, -1.f, 0.0f,	//v1
        0.0f, 1.0f,

        -1.f, 1.f, 0.0f, //v2
        0.0f,  0.0f,

        //t2
        -1.f, 1.f, 0.0f,	//v0 pos
        0.0f, 0.0f,

        1.f, 1.f, 0.0f,	//v1
        1.0f, 0.0f,

        1.f, -1.f, 0.0f, //v2
        1.0f, 1.0f
    };


    D3D11_BUFFER_DESC desc = { 0 };

    desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    desc.ByteWidth = sizeof(TriangleVertex) * 6;

    D3D11_SUBRESOURCE_DATA data = { 0 };
    data.pSysMem = triangleVertices;

    ThrowIfFailed(device->CreateBuffer(&desc, &data, &menuQuad));

    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.Usage = D3D11_USAGE_DYNAMIC;

    ThrowIfFailed(device->CreateBuffer(&desc, &data, &buttonQuad));


}

