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
#include "DoF.h"
#include "SnowManager.h"

#include <SpriteBatch.h>


namespace Graphics
{
	class Renderer
    {
    public:

        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera);
        virtual ~Renderer();
        void initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, Camera * camera);


        void render(Camera * camera);
        void queueRender(RenderInfo * renderInfo);
		void queueFoliageRender(FoliageRenderInfo * renderInfo);
		void queueWaterRender(WaterRenderInfo * renderInfo);
        void queueRenderDebug(RenderDebugInfo * debugInfo);
        void queueText(TextString * text);
		void queueLight(Light light);
		void fillHUDInfo(HUDInfo * info);

        void drawMenu(Graphics::MenuInfo * info);
        void updateLight(float deltaTime, Camera * camera);

        //indicates how gray the screen will be
        void setBulletTimeCBuffer(float value);

        void updateShake(float deltaTime);
        void startShake(float radius, float duration);
    private:
        typedef  std::unordered_map<ModelID, std::vector<InstanceData>> InstanceQueue_t;
        InstanceQueue_t instanceQueue;
        std::vector<RenderInfo*> renderQueue;
		std::vector<FoliageRenderInfo*> renderFoliageQueue;
		std::vector<WaterRenderInfo*> renderWaterQueue;


        DepthStencil depthStencil;

        SkyRenderer skyRenderer;
        Glow glowRenderer;

		LightGrid grid;
		std::vector<Light> lights;

		DirectX::CommonStates *states;

        Shader fullscreenQuad;
        Shader forwardPlus;
		Shader depthShader;

        //ComputeShader lightGridGen; 

        StructuredBuffer<InstanceData> instanceSBuffer;
        ConstantBuffer<UINT> instanceOffsetBuffer;
        ResourceManager resourceManager;
        D3D11_VIEWPORT viewPort;

        // Lånade Pekareu
        ID3D11Device * device;
        ID3D11DeviceContext * deviceContext;
        ID3D11RenderTargetView * backBuffer;


		SSAORenderer ssaoRenderer;
        

		ShaderResource* fakeBackBuffer;
		ShaderResource* fakeBackBufferSwap;

        ID3D11BlendState *transparencyBlendState;

        
		bool enablePostEffects = true;

		bool enableSSAO = true;
		bool enableGlow = true;
		bool enableFog = true;
		bool enableDOF = true;
        bool enableCoCWindow = false;

        Menu menu;
        HUD hud;
        DoF DoFRenderer;

        ConstantBuffer<float> bulletTimeBuffer;

        //temp
        ID3D11ShaderResourceView * glowTest;

		SnowManager snowManager;


		//superTemp
		struct StatusData
		{
			float burn;
			float freeze;
		} statusData;
       
        void cull();
        void writeInstanceData();
        void draw();
		void clear();
		void swapBackBuffers();
		
#pragma region Foliage
		 
		ConstantBuffer <UINT> timeBuffer;
		UINT grassTime = 0;
		void drawFoliage(Camera * camera);
		Shader foliageShader;
#pragma endregion

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);
        void createBlendState();
		void registerDebugFunction();


    #pragma region RenderDebugInfo

        Shader debugRender;
        std::vector<RenderDebugInfo*> renderDebugQueue;
        StructuredBuffer<DirectX::SimpleMath::Vector3> debugPointsBuffer;
        ConstantBuffer<DirectX::SimpleMath::Color> debugColorBuffer;
        void renderDebugInfo(Camera* camera);

    #pragma endregion
		Fog fog;

    };
};