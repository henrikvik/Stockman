#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "Camera.h"
#include "Structs.h"
#include "Datatypes.h"
#include "WICTextureLoader.h"
#include "Lights\LightGrid.h"
#include "Resources\ResourceManager.h"
#include "Resources\DepthStencil.h"
#include "Resources\ResourceManager.h"
#include "Resources\Shader.h"
#include "Utility\ConstantBuffer.h"
#include "Utility\StructuredBuffer.h"
#include "SkyRenderer.h"

#include <SpriteBatch.h>

namespace Graphics
{
    class Renderer
    {
    public:
        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera);
		virtual ~Renderer();
        void render(Camera * camera);
        void drawMenu(Graphics::MenuInfo * info);
        void queueRender(RenderInfo * renderInfo);
        void initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext);

		void updateLight(float deltaTime, Camera * camera);
    private:
        typedef  std::unordered_map<ModelID, std::vector<InstanceData>> InstanceQueue_t;
        std::vector<RenderInfo*> renderQueue;
        InstanceQueue_t instanceQueue;

        DepthStencil depthStencil;

		SkyRenderer skyRenderer;

		LightGrid grid;
		DirectX::CommonStates *states;

        Shader fullscreenQuad;
        Shader forwardPlus;
        Shader menuShader;
        Shader GUIShader;

        //ComputeShader lightGridGen; 

        StructuredBuffer<InstanceData> instanceSBuffer;
        ConstantBuffer<UINT> instanceOffsetBuffer;
        ResourceManager resourceManager;
        D3D11_VIEWPORT viewPort;

        // Lånade Pekare
        ID3D11Device * device;
        ID3D11DeviceContext * deviceContext;
        ID3D11RenderTargetView * backBuffer;

        ///// SUPER TEMP
       
       
		

	




        ID3D11ShaderResourceView * menuTexture;
        //crosshair
        ID3D11ShaderResourceView * GUITexture1;
        //HP bar
        ID3D11ShaderResourceView * GUITexture2;
        ID3D11ShaderResourceView * buttonTexture;
       

        ID3D11Buffer *GUIvb;
        ID3D11BlendState *transparencyBlendState;

        ID3D11Buffer * menuQuad;
        ID3D11Buffer * buttonQuad;



        void loadModellessTextures();

        void cull();
        void writeInstanceData();
        void draw();
        void drawGUI();
		

        void mapButtons(ButtonInfo * info);


        
		

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);

        void createBlendState();
        void createGUIBuffers();

        void createMenuVBS();
    };
};