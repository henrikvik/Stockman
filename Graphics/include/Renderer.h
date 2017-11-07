#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "Camera.h"
#include "Datatypes.h"
#include "WICTextureLoader.h"
#include "Lights\LightGrid.h"
#include "Utility\DepthStencil.h"
#include "Utility\ConstantBuffer.h"
#include "Utility\StructuredBuffer.h"
#include "Utility\ShaderResource.h"
#include "Lights\Sun.h"
#include "Menu.h"
#include "HUD.h"
#include "HybrisLoader\HybrisLoader.h"
#include "SSAORenderer.h"
#include "Fog.H"
#include "DoF.h"

#include <SpriteBatch.h>

#include "RenderPass\GUIRenderPass.h"


namespace Graphics
{
	class Renderer
    {
    public:

        Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera);
        virtual ~Renderer();
        void initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, Camera * camera);


        void render(Camera * camera);
		void fillHUDInfo(HUDInfo * info);

        void drawMenu(Graphics::MenuInfo * info);
        void updateLight(float deltaTime, Camera * camera);

        //indicates how gray the screen will be
        void setBulletTimeCBuffer(float value);

        void updateShake(float deltaTime);
        void startShake(float radius, float duration);
    private:

    #pragma region Shared Shader Resources
        std::vector<RenderPass*> renderPasses;
        ShaderResource colorMap;
        ShaderResource glowMap;
        ShaderResource normalMap;
        DepthStencil shadowMap;
        DepthStencil depthStencil;

        ID3D11ShaderResourceView  * lightOpaqueGridSRV;
        ID3D11UnorderedAccessView * lightOpaqueGridUAV;
        StructuredBuffer<uint32_t> lightOpaqueIndexList;
        StructuredBuffer<Light>    lightsNew;

        ID3D11RenderTargetView * backBuffer;

        ShaderResource* fakeBackBuffer;
        ShaderResource* fakeBackBufferSwap;
    #pragma endregion
        SSAORenderer ssaoRenderer;
        Sun sun;
        Shader fullscreenQuad;
        Shader forwardPlus;
		Shader depthShader;

        D3D11_VIEWPORT viewPort;
        
		bool enableSSAO = true;
		bool enableGlow = true;
		bool enableFog = false;
		bool enableDOF = false;
        bool enableCoCWindow = false;
        bool enableSnow = true;
        bool enableHud = true;

        Menu menu;
        HUD hud;
        DoF DoFRenderer;

        ConstantBuffer<float> bulletTimeBuffer;

        //temp
        ID3D11ShaderResourceView * glowTest;

		//superTemp
		struct StatusData
		{
			float burn;
			float freeze;
		} statusData;

		void clear();
		void swapBackBuffers();
		
#pragma region Foliage
		 
		ConstantBuffer <UINT> timeBuffer;
		UINT grassTime = 0;
		Shader foliageShader;
#pragma endregion

        void drawToBackbuffer(ID3D11ShaderResourceView * texture);

		void registerDebugFunction();


    #pragma region RenderDebugInfo

        Shader debugRender;
        std::vector<RenderDebugInfo*> renderDebugQueue;
        StructuredBuffer<DirectX::SimpleMath::Vector3> debugPointsBuffer;
        ConstantBuffer<DirectX::SimpleMath::Color> debugColorBuffer;
        void renderDebugInfo(Camera* camera);

    #pragma endregion
		Fog fog;

    #pragma region Draw Functions and Buffers


        StructuredBuffer<InstanceData> instanceSBuffer;
        ConstantBuffer<UINT> instanceOffsetBuffer;

        void writeInstanceBuffers();
        StructuredBuffer<InstanceData> staticInstanceBuffer;

    #pragma endregion



    };
};