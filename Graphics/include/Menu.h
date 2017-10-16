#pragma once
#include "Resources\Shader.h"
#include <Engine\Constants.h>
#include "Utility\ShaderResource.h"
#include <CommonStates.h>
#include "Structs.h"

namespace Graphics
{
    class Menu
    {
    public:
        Menu(ID3D11Device * device, ID3D11DeviceContext * contex);
        ~Menu();

        void drawMenu(ID3D11Device * device, ID3D11DeviceContext * contex, Graphics::MenuInfo * info, ID3D11RenderTargetView * backBuffer, ID3D11BlendState * blendState);
        
        void unloadTextures();


    private:
        void mapButtons(ID3D11DeviceContext * contex, Graphics::MenuInfo * info);
        void createVBuffers(ID3D11Device * device);
        void loadTextures(ID3D11Device * device, ID3D11DeviceContext * contex);
        

        ID3D11ShaderResourceView * menuTexture[4];
        ID3D11ShaderResourceView * buttonTexture[2];
        ID3D11Buffer * buttonQuad;
        ID3D11Buffer * menuQuad;

        DirectX::CommonStates * states;

        Shader shader;
        MenuInfo * active;
        bool loaded;
    };
}



// moves the buttons to ndc space
//TriangleVertex triangleVertices[6] =
//{
//    2 * ((float)((info->m_rek.x + info->m_rek.width)) / WIN_WIDTH) - 1, 2 * ((float)(WIN_HEIGHT - info->m_rek.y - info->m_rek.height) / WIN_HEIGHT) - 1, 0.0f,	//v0 pos
//    info->m_texCoordEnd.x, info->m_texCoordEnd.y,

//    2 * ((float)(info->m_rek.x) / WIN_WIDTH) - 1 , 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height)) / WIN_HEIGHT) - 1, 0.0f,	//v1
//    info->m_texCoordStart.x, info->m_texCoordEnd.y,

//    2 * ((float)(info->m_rek.x) / WIN_WIDTH) - 1 , 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height + info->m_rek.height)) / WIN_HEIGHT) - 1, 0.0f, //v2
//    info->m_texCoordStart.x,  info->m_texCoordStart.y,

//    //t2
//    2 * ((float)(info->m_rek.x) / WIN_WIDTH) - 1 , 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height + info->m_rek.height)) / WIN_HEIGHT) - 1, 0.0f,	//v2 pos
//    info->m_texCoordStart.x, info->m_texCoordStart.y,

//    2 * ((float)((info->m_rek.x + info->m_rek.width)) / WIN_WIDTH) - 1, 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height + info->m_rek.height)) / WIN_HEIGHT) - 1 , 0.0f,	//v3
//    info->m_texCoordEnd.x, info->m_texCoordStart.y,

//    2 * ((float)((info->m_rek.x + info->m_rek.width)) / WIN_WIDTH) - 1, 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height)) / WIN_HEIGHT) - 1 , 0.0f, //v0
//    info->m_texCoordEnd.x, info->m_texCoordEnd.y,
//};