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
#include "Utility\DepthStencil.h"
#include "Resources\ResourceManager.h"
#include "Utility\ConstantBuffer.h"
#include "Utility\StructuredBuffer.h"
#include "Utility\ShaderResource.h"
#include "Glow.h";
#include "SkyRenderer.h"
#include "Menu.h"
#include "HUD.h"
#include "SSAORenderer.h"
#include "Fog.H"

#include <SpriteBatch.h>


namespace Graphics
{
    class Renderer
    {
    public:
        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera);
		virtual ~Renderer();
        void initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext);


        void render(Camera * camera);
        void queueRender(RenderInfo * renderInfo);
        void queueRenderDebug(RenderDebugInfo * debugInfo);
        void queueText(TextString * text);
        void fillHUDInfo(HUDInfo * info);

        void drawMenu(Graphics::MenuInfo * info);
		void updateLight(float deltaTime, Camera * camera);

		//indicates how gray the screen will be
		void setBulletTimeCBuffer(float value);

		void updateShake(float deltaTime);
    private:
        typedef  std::unordered_map<ModelID, std::vector<InstanceData>> InstanceQueue_t;
        InstanceQueue_t instanceQueue;
        std::vector<RenderInfo*> renderQueue;

        DepthStencil depthStencil;

		SkyRenderer skyRenderer;
		Glow postProcessor;

		LightGrid grid;
		DirectX::CommonStates *states;

        Shader fullscreenQuad;
        Shader forwardPlus;

        //ComputeShader lightGridGen; 

        StructuredBuffer<InstanceData> instanceSBuffer;
        ConstantBuffer<UINT> instanceOffsetBuffer;
        ResourceManager resourceManager;
        D3D11_VIEWPORT viewPort;

        // Lånade Pekare
        ID3D11Device * device;
        ID3D11DeviceContext * deviceContext;
        ID3D11RenderTargetView * backBuffer;


		SSAORenderer ssaoRenderer;
        

		ShaderResource fakeBackBuffer;
		ShaderResource fakeBackBufferSwap;
		ShaderResource glowMap;

        ID3D11BlendState *transparencyBlendState;


        Menu menu;
        HUD hud;


		ConstantBuffer<float> bulletTimeBuffer;


		//temp
		ID3D11ShaderResourceView * glowTest;

       
        void cull();
        void writeInstanceData();
        void draw();
        void drawGUI();
		



        
		

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);

        void createBlendState();



    #pragma region RenderDebugInfo

        Shader debugRender;
        std::vector<RenderDebugInfo*> renderDebugQueue;
        StructuredBuffer<DirectX::SimpleMath::Vector3> debugPointsBuffer;
        ConstantBuffer<DirectX::SimpleMath::Color> debugColorBuffer;
        void renderDebugInfo(Camera* camera);

    #pragma endregion
		Fog fog;
		ShaderResource worldPosMap;




    };
};