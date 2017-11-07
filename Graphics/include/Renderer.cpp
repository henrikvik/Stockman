#include "Renderer.h"
#include <stdio.h>
#include <Graphics\include\ThrowIfFailed.h>
#include <Engine\Constants.h>
#include "Utility\sizeofv.h"
#include <Keyboard.h>
#include <Engine\DebugWindow.h>

#include <Engine\Profiler.h>
#include "RenderQueue.h"

#include "Particles\ParticleSystem.h"
#include "Utility\TextureLoader.h"
#include "CommonStates.h"
#include "MainCamera.h"

#include "RenderPass\ForwardPlusRenderPass.h"
#include "RenderPass\DepthRenderPass.h"
#include "RenderPass\LightCullRenderPass.h"


#define USE_TEMP_CUBE false
#define ANIMATION_HIJACK_RENDER false
#define USE_OLD_RENDER false

#if USE_TEMP_CUBE
#include "TempCube.h"
#endif


#define MAX_DEBUG_POINTS 10000
#define _INSTANCE_CAP 300

namespace Graphics
{
    uint32_t zero = 0;
	Renderer::Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera)
		: forwardPlus(device, Resources::Shaders::ForwardPlus)
		, fullscreenQuad(device, SHADER_PATH("FullscreenQuad.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R8_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } })
		, depthStencil(device, WIN_WIDTH, WIN_HEIGHT)
		, instanceSBuffer(device, CpuAccess::Write, _INSTANCE_CAP)
		, instanceOffsetBuffer(device)
		, glowRenderer(device, deviceContext)
#pragma region RenderDebugInfo
        , debugPointsBuffer(device, CpuAccess::Write, MAX_DEBUG_POINTS)
        , debugRender(device, SHADER_PATH("DebugRender.hlsl"))
        , debugColorBuffer(device)
#pragma endregion
        , fog(device)
        , menu(device, deviceContext)
        , hud(device, deviceContext)
        , ssaoRenderer(device)
        , bulletTimeBuffer(device)
        , DoFRenderer(device)
#pragma region Foliage
        , foliageShader(device, SHADER_PATH("FoliageShader.hlsl"), VERTEX_DESC)
        , timeBuffer(device)


#pragma endregion
		, depthShader(device, SHADER_PATH("DepthPixelShader.hlsl"), {}, ShaderType::PS)
        , staticInstanceBuffer(device, CpuAccess::Write, INSTANCE_CAP(StaticRenderInfo))

    #pragma region Shared Shader Resources
        , colorMap(WIN_WIDTH, WIN_HEIGHT)
        , glowMap(WIN_WIDTH, WIN_HEIGHT)
        , normalMap(WIN_WIDTH, WIN_HEIGHT)

        , shadowMap(Global::device, SHADOW_RESOLUTION, SHADOW_RESOLUTION)


        , lightOpaqueIndexList(CpuAccess::None, 1, &zero)
        , lightsNew(CpuAccess::Write, INSTANCE_CAP(LightRenderInfo))
        , sun()
    #pragma endregion


    {
        Global::cStates = new CommonStates(device);
        Global::comparisonSampler = [&](){
            ID3D11SamplerState * sampler = nullptr;
            D3D11_SAMPLER_DESC sDesc = {};
            sDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
            sDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
            sDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
            sDesc.BorderColor[0] = 1;
            sDesc.BorderColor[1] = 1;
            sDesc.BorderColor[2] = 1;
            sDesc.BorderColor[3] = 1;
            sDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
            sDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
            sDesc.MaxAnisotropy = 0;
            sDesc.MinLOD = 0;
            sDesc.MaxLOD = D3D11_FLOAT32_MAX;
            sDesc.MipLODBias = 0;

            ThrowIfFailed(device->CreateSamplerState(&sDesc, &sampler));
            return sampler;        
        }();

        { // CaNCeR!
            D3D11_TEXTURE2D_DESC desc = {};
            desc.Format = DXGI_FORMAT_R32G32_UINT;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
            desc.Width = (int)ceil(1280 / (float)BLOCK_SIZE);
            desc.Height = (int)ceil(720 / (float)BLOCK_SIZE);
            desc.SampleDesc.Count = 1;
            desc.MipLevels = 1;
            desc.ArraySize = 1;

            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            ZeroMemory(&uavDesc, sizeof(uavDesc));
            uavDesc.Format = DXGI_FORMAT_R32G32_UINT;
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.Format = DXGI_FORMAT_R32G32_UINT;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;

            ID3D11Texture2D *texture;
            ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &texture));
            ThrowIfFailed(device->CreateUnorderedAccessView(texture, &uavDesc, &lightOpaqueGridUAV));
            ThrowIfFailed(device->CreateShaderResourceView(texture, &srvDesc, &lightOpaqueGridSRV));
            SAFE_RELEASE(texture);
        }

		this->backBuffer = backBuffer;

		initialize(device, deviceContext, camera);

		fakeBackBuffer = newd ShaderResource(device, WIN_WIDTH, WIN_HEIGHT);
		fakeBackBufferSwap = newd ShaderResource(device, WIN_WIDTH, WIN_HEIGHT);

		viewPort = { 0 };
		viewPort.Width = WIN_WIDTH;
		viewPort.Height = WIN_HEIGHT;
		viewPort.MaxDepth = 1.0f;

        FXSystem = newd ParticleSystem(device, 512, "Resources/Particles/base.part");

        float temp = 1.f;
        bulletTimeBuffer.write(deviceContext, &temp, sizeof(float));


		registerDebugFunction();

		statusData.burn = 0;
		statusData.freeze = 0;

        TextureLoader::get().loadAll();

        renderPasses =
        {
            newd DepthRenderPass({}, {staticInstanceBuffer}, {*Global::mainCamera->getBuffer()}, depthStencil),
            newd ShadowRenderPass({}, { staticInstanceBuffer }, {*sun.getLightMatrixBuffer()}, shadowMap),
            newd LightCullRenderPass(
                {},
                {
                    depthStencil,
                    lightsNew
                },
                {
                    *Global::mainCamera->getBuffer()
                },
                nullptr,
                {
                    lightOpaqueIndexList,
                    lightOpaqueGridUAV
                }
            ),
            newd ForwardPlusRenderPass(
                {
                    backBuffer,
                    glowMap,
                    normalMap
                },
                {
                    lightOpaqueIndexList,
                    lightOpaqueGridSRV,
                    lightsNew,
                    shadowMap,
                    staticInstanceBuffer
                },
                {
                    *Global::mainCamera->getBuffer(),
                    *sun.getLightDataBuffer(),
                    *sun.getLightMatrixBuffer()
                },
                depthStencil
            ),
            newd GUIRenderPass({backBuffer}),
        };
    }


    Renderer::~Renderer()
    {
		delete fakeBackBuffer;
		delete fakeBackBufferSwap;
        delete FXSystem;

        delete Global::cStates;
        SAFE_RELEASE(Global::comparisonSampler);
		SAFE_RELEASE(glowTest);
        SAFE_RELEASE(lightOpaqueGridUAV);
        SAFE_RELEASE(lightOpaqueGridSRV);

        for (auto & renderPass : renderPasses)
        {
            delete renderPass;
        }

    }

    void Renderer::initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext, Camera * camera)
    {
        //resourceManager.initialize(gDevice, gDeviceContext);
		//skyRenderer.initialize(resourceManager.getModelInfo(SKY_SPHERE));

        //temp
        DirectX::CreateWICTextureFromFile(Global::device, TEXTURE_PATH("glowMapTree.png"), NULL, &glowTest);
    }

	void Renderer::updateLight(float deltaTime, Camera * camera)
	{
		//Temp or rename function
		PROFILE_BEGIN("updateSnow()");
		PROFILE_END();
	}

	//this function is called in SkillBulletTime.cpp
	void Renderer::setBulletTimeCBuffer(float amount)
	{
		PROFILE_BEGIN("SetBulletTimeCBuffer()");
		//These two must always add up to one ir i'll have to fix the formula
		//They represents how long the fade in and fade out are. 
		static const float TOP_THRESHOLD = 0.9f;
		static const float BOT_THRESHOLD = 0.1f;


        if (amount > TOP_THRESHOLD)
            amount = (amount - TOP_THRESHOLD) / BOT_THRESHOLD;

        else if (amount < BOT_THRESHOLD)
            amount = 1 - ((amount) / BOT_THRESHOLD);

        else amount = 0;

        bulletTimeBuffer.write(Global::context, &amount, sizeof(float));
        PROFILE_END();
    }

	void Renderer::updateShake(float deltaTime)
	{
		PROFILE_BEGIN("UpdateShake()");
		hud.updateShake(Global::context, deltaTime);
		PROFILE_END();
	}

	//Radius is in pixels on screen, duration is in MS
    void Renderer::startShake(float radius, float duration)
    {
        hud.startShake(radius, duration);
    }

    void Renderer::render(Camera * camera)
#if ANIMATION_HIJACK_RENDER
    {
        static Shader shader(device, Resources::Shaders::ForwardPlus);
        static HybrisLoader::Model * model = hybrisLoader.getModel(Resources::Models::Cube);
        static float clearColor[4] = {0,0,0,0};

		renderQueue.clear();
		static Camera cam(device, WIN_WIDTH, WIN_HEIGHT);
		static UINT ticks = 0;
		ticks++;
		cam.updateLookAt({ 5 * sinf(ticks * 0.001f), 5 * cosf(ticks * 0.001f), 5 }, { 0,0,0 }, deviceContext);

        deviceContext->ClearRenderTargetView(backBuffer, clearColor);
        deviceContext->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1, 0);

        deviceContext->IASetInputLayout(nullptr);
        deviceContext->VSSetShader(shader, nullptr, 0);
        deviceContext->PSSetShader(shader, nullptr, 0);

        deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->RSSetState(states->CullClockwise());
        deviceContext->OMSetRenderTargets(1, &backBuffer, depthStencil);

        deviceContext->VSSetConstantBuffers(0, 1, *camera);
        deviceContext->PSSetConstantBuffers(0, 1, *camera);

        static ID3D11SamplerState * sampler = states->PointClamp();
        deviceContext->PSSetSamplers(0, 1, &sampler);

        writeInstanceBuffers();

        drawStatic();

        RenderQueue::get().clearAllQueues();
    }
#else
    #if USE_OLD_RENDER
	{
        PROFILE_BEGIN("writeInstanceBuffers()");
        writeInstanceBuffers();
        PROFILE_END();

        menu.unloadTextures();

		PROFILE_BEGIN("clear()");
		clear();
		PROFILE_END();

		//PROFILE_BEGIN("Cull()");
		//cull();
		//PROFILE_END();

		//PROFILE_BEGIN("WriteInstanceData()");
		//writeInstanceData();
		//PROFILE_END();

        PROFILE_BEGIN("FXSystem::update()")
        FXSystem->update(deviceContext, camera, 0.016f);
        PROFILE_END();

		PROFILE_BEGIN("drawShadows()");
		deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);
		//Drawshadows does not actually draw anything, it just sets up everything for drawing shadows
		skyRenderer.drawShadows(deviceContext, &forwardPlus);
        drawStatic();
		PROFILE_END();


		PROFILE_BEGIN("depthPass");

		deviceContext->PSSetConstantBuffers(0, 1, *camera->getBuffer());
		deviceContext->VSSetConstantBuffers(0, 1, *camera->getBuffer());

		


		deviceContext->RSSetViewports(1, &viewPort);
		deviceContext->RSSetState(states->CullCounterClockwise());

		deviceContext->IASetInputLayout(nullptr);
		deviceContext->VSSetShader(forwardPlus, nullptr, 0);

        deviceContext->PSSetShader(nullptr, nullptr, 0);
        deviceContext->OMSetRenderTargets(0, nullptr, depthStencil);

		drawStatic();

        // FIIIIIX
		////////////deviceContext->IASetInputLayout(foliageShader);
		////////////deviceContext->VSSetShader(foliageShader, nullptr, 0);
		////////////deviceContext->PSSetShader(depthShader, nullptr, 0);
		////////////
		//////////////this be no deltatime
		////////////grassTime++;

		////////////drawFoliage(camera);

		//drawFoliage(camera);
	
        PROFILE_BEGIN("FXSystem::renderPrePass()")
        FXSystem->renderPrePass(deviceContext, camera, states, depthStencil);
        PROFILE_END();
        
        PROFILE_END();

		PROFILE_BEGIN("grid.updateLights()");
		deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		deviceContext->RSSetState(states->CullCounterClockwise());

		grid.updateLights(deviceContext, camera, lights);
		lights.clear();
		PROFILE_END();

		PROFILE_BEGIN("grid.cull()");
		grid.cull(camera, states, depthStencil, device, deviceContext);
		PROFILE_END();

        PROFILE_BEGIN("FXSystem::render()")
        FXSystem->render(deviceContext, camera, states, *fakeBackBuffer, depthStencil, false);
        PROFILE_END();

		PROFILE_BEGIN("draw()");
		deviceContext->IASetInputLayout(nullptr);
		deviceContext->VSSetShader(forwardPlus, nullptr, 0);
		deviceContext->PSSetShader(forwardPlus, nullptr, 0);

		ID3D11ShaderResourceView *SRVs[] = {
			grid.getOpaqueIndexList()->getSRV(),
			grid.getOpaqueLightGridSRV(),
			grid.getLights()->getSRV(),
			*skyRenderer.getDepthStencil()
		};
		auto sampler = states->LinearClamp();
		deviceContext->PSSetShaderResources(0, 4, SRVs);
		deviceContext->PSSetSamplers(0, 1, &sampler);

		auto samplerWrap = states->LinearWrap();
		deviceContext->PSSetSamplers(2, 1, &samplerWrap);

		deviceContext->PSSetSamplers(1, 1, skyRenderer.getSampler());

		ID3D11Buffer *lightBuffs[] =
		{
			*skyRenderer.getShaderBuffer(),
			*skyRenderer.getLightMatrixBuffer()
		};

		deviceContext->PSSetConstantBuffers(1, 1, &lightBuffs[0]);
		deviceContext->VSSetConstantBuffers(3, 1, &lightBuffs[1]);

        deviceContext->PSSetConstantBuffers(2, 1, bulletTimeBuffer);

		ID3D11RenderTargetView * rtvs[] =
		{
			*fakeBackBuffer,
			glowRenderer,
			*ssaoRenderer.getNormalShaderResource()
		};

		deviceContext->OMSetRenderTargets(3, rtvs, depthStencil);
		
        drawStatic();
		PROFILE_END();

        // FIIX
		//////////PROFILE_BEGIN("RenderFoliage");
		//////////deviceContext->IASetInputLayout(foliageShader);
		//////////deviceContext->VSSetShader(foliageShader, nullptr, 0);
		//////////deviceContext->PSSetShader(foliageShader, nullptr, 0);
		//////////drawFoliage(camera);
		//////////renderFoliageQueue.clear();
		//////////PROFILE_END();


	/*	PROFILE_BEGIN("RenderWater");
		drawWater(camera);
		PROFILE_END();*/

		//The sky renderer uses the bullet time on register 3
		deviceContext->PSSetConstantBuffers(3, 1, bulletTimeBuffer);
        deviceContext->RSSetState(states->CullCounterClockwise());
        skyRenderer.renderSky(deviceContext, camera);

       

        ID3D11RenderTargetView * rtvNULL[3] = { nullptr };

        deviceContext->OMSetRenderTargets(3, rtvNULL, nullptr);

        ZeroMemory(SRVs, sizeof(SRVs));
        deviceContext->PSSetShaderResources(0, 4, SRVs);

        PROFILE_BEGIN("DebugThings");

		deviceContext->RSSetState(states->CullCounterClockwise());
		SHORT tabKeyState = GetAsyncKeyState(VK_TAB);
		if ((1 << 15) & tabKeyState)
		{
			this->drawToBackbuffer(grid.getDebugSRV());
		}
		PROFILE_END();

		//TEEEMP
		auto ks = DirectX::Keyboard::Get().GetState();

		
        bool enablePostEffects = false;
        if (enablePostEffects)
        {

            ///////Post effects

            if (enableDOF)
            {
                PROFILE_BEGIN("Dof");

                if (enableCoCWindow)
                {
                    ImGui::Begin("camera stuff");
                    static float fp = 0.088f;
                    static float fl = 0.05f;
                    static float a = 0.12f;
                    ImGui::SliderFloat("focal Plane", &fp, 0.0001f, .1f);
                    ImGui::SliderFloat("focal lenght", &fl, 0.001f, 1.0f);
                    ImGui::SliderFloat("apature", &a, 0.001f, 1.0f);
                    DoFRenderer.updateCoc(deviceContext, fl, fp, a);
                    ImGui::End();
                }
                DoFRenderer.DoFRender(deviceContext, fakeBackBuffer, &depthStencil, fakeBackBufferSwap, camera);
                swapBackBuffers();
                PROFILE_END();
            }

            if (enableGlow)
            {
                PROFILE_BEGIN("Glow");
                glowRenderer.addGlow(deviceContext, *fakeBackBuffer, fakeBackBufferSwap);
                swapBackBuffers();
                PROFILE_END();
            }

            if (enableSSAO)
            {
                PROFILE_BEGIN("SSAO");
                ssaoRenderer.renderSSAO(deviceContext, camera, &depthStencil, fakeBackBuffer, fakeBackBufferSwap);
                swapBackBuffers();
                PROFILE_END();
            }
        }

			
        //The last post effect should just write to backbuffer instead of this 
        //unnecessary draw call
		static float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static UINT sampleMask = 0xffffffff;
		deviceContext->OMSetBlendState(transparencyBlendState, blendFactor, sampleMask);

		PROFILE_BEGIN("DrawToBackBuffer");
		drawToBackbuffer(*fakeBackBuffer);
		PROFILE_END();


		if (enableFog)
		{
			PROFILE_BEGIN("renderFog()");

			deviceContext->PSSetConstantBuffers(0, 1, *camera->getBuffer());
			deviceContext->PSSetConstantBuffers(1, 1, *camera->getInverseBuffer());
			fog.renderFog(deviceContext, backBuffer, depthStencil);
			PROFILE_END();
		}

        if (enableSnow)
        {
            //Needs the light for snowflakes
            ID3D11ShaderResourceView *SRVs[] = {
                grid.getOpaqueIndexList()->getSRV(),
                grid.getOpaqueLightGridSRV(),
                grid.getLights()->getSRV(),
            };
            deviceContext->PSSetShaderResources(0, 3, SRVs);

            auto state = states->DepthDefault();
            deviceContext->OMSetDepthStencilState(state, 0);
            auto state2 = states->CullNone();
            deviceContext->RSSetState(state2);
            
            static float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            static UINT sampleMask = 0xffffffff;
            
            deviceContext->OMSetBlendState(transparencyBlendState, blendFactor, sampleMask);
            snowManager.drawSnowflakes(deviceContext, camera, backBuffer, &depthStencil, skyRenderer);
            deviceContext->GSSetShader(nullptr, nullptr, 0);
            deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            
            ZeroMemory(SRVs, sizeof(SRVs));
            deviceContext->PSSetShaderResources(0, 3, SRVs);
        }

        if (enableHud)
        {
            PROFILE_BEGIN("HUD");
            //hud.drawHUD(deviceContext, backBuffer, transparencyBlendState);
            PROFILE_END();
        }

		PROFILE_BEGIN("DebugInfo");
		renderDebugInfo(camera);
		PROFILE_END();

		if (ks.G)
		{
			startShake(30, 1000);
		}

        
        InstanceBuffers::get().writeQueueToBuffer<StaticRenderInfo, StaticInstanceData>();



        PROFILE_BEGIN("RenderPasses");
        for (auto & renderPass : renderPasses)
        {
            renderPass->render();
        }
        PROFILE_END();
	}
    #else
    {
        static float clearColor[4] = {0,0,0,0};
        clear();
        Global::context->RSSetViewports(1, &viewPort);

        writeInstanceBuffers();

        sun.update();

        for (auto & renderPass : renderPasses)
        {
            renderPass->update(0.016f);
        }

        for (auto & renderPass : renderPasses)
        {
            renderPass->render();
        }
    }
    #endif
#endif

    void Renderer::fillHUDInfo(HUDInfo * info)
    {
        hud.fillHUDInfo(info);
    }

	void Renderer::clear()
	{
		static float clearColor[4] = { 0 };
		Global::context->ClearRenderTargetView(backBuffer, clearColor);
		Global::context->ClearRenderTargetView(*fakeBackBuffer, clearColor);
		Global::context->ClearRenderTargetView(glowRenderer, clearColor);
        Global::context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.f, 0);
        Global::context->ClearDepthStencilView(shadowMap, D3D11_CLEAR_DEPTH, 1.f, 0);
		glowRenderer.clear(Global::context, clearColor);
	}

	void Renderer::swapBackBuffers()
	{
		ShaderResource * temp = fakeBackBuffer;
		fakeBackBuffer = fakeBackBufferSwap;
		fakeBackBufferSwap = temp;
	}

    void Renderer::drawToBackbuffer(ID3D11ShaderResourceView * texture)
    {
        Global::context->PSSetShaderResources(0, 1, &texture);

        UINT zero = 0;
        Global::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        Global::context->OMSetRenderTargets(1, &backBuffer, nullptr);

        Global::context->IASetInputLayout(fullscreenQuad);
        Global::context->VSSetShader(fullscreenQuad, nullptr, 0);
        Global::context->PSSetShader(fullscreenQuad, nullptr, 0);

        static ID3D11SamplerState * pointClamp = Global::cStates->PointClamp();
        Global::context->PSSetSamplers(0, 1, &pointClamp);

		PROFILE_BEGIN("Draw(4, 0)");
        Global::context->Draw(4, 0);
		PROFILE_END();

        ID3D11ShaderResourceView * srvNull = nullptr;
        Global::context->PSSetShaderResources(0, 1, &srvNull);
    }

    //draws the menu
    void Renderer::drawMenu(Graphics::MenuInfo * info)
    {
        Global::context->RSSetViewports(1, &viewPort);
        menu.drawMenu(Global::device, Global::context, info, backBuffer, Global::cStates->AlphaBlend());
        hud.renderText(Global::cStates->AlphaBlend(), false);

    }


    void Renderer::renderDebugInfo(Camera* camera)
    {
        if (renderDebugQueue.size() == 0) return;

        Global::context->PSSetConstantBuffers(0, 1, *camera->getBuffer());
        Global::context->VSSetConstantBuffers(0, 1, *camera->getBuffer());

        Global::context->OMSetRenderTargets(1, &backBuffer, depthStencil);

        Global::context->VSSetShaderResources(0, 1, debugPointsBuffer);
        Global::context->PSSetConstantBuffers(1, 1, debugColorBuffer);

        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(debugRender, nullptr, 0);
        Global::context->PSSetShader(debugRender, nullptr, 0);


        for (RenderDebugInfo * info : renderDebugQueue)
        {
            debugPointsBuffer.write(
                Global::context,
                info->points->data(),
                (UINT)(info->points->size() * sizeof(DirectX::SimpleMath::Vector3))
            );

            debugColorBuffer.write(
                Global::context,
                &info->color,
                (UINT)sizeof(DirectX::SimpleMath::Color)
            );

            Global::context->IASetPrimitiveTopology(info->topology);
            Global::context->OMSetDepthStencilState(info->useDepth ? Global::cStates->DepthDefault() : Global::cStates->DepthNone(), 0);
            Global::context->Draw((UINT)info->points->size(), 0);
        }

        renderDebugQueue.clear();
    }

    void Renderer::writeInstanceBuffers()
    {
        InstanceData * instanceBuffer = staticInstanceBuffer.map(Global::context);
        for (auto & model_infos : RenderQueue::get().getQueue<StaticRenderInfo>())
        {
            for (auto & sinfo : model_infos.second)
            {
                InstanceData instanceData = {};
                instanceData.transform = sinfo->transform;
                instanceData.transformInvT = sinfo->transform.Invert().Transpose();


                *instanceBuffer++ = instanceData;
            }
        }
        staticInstanceBuffer.unmap(Global::context);

        Light * lightBuffer = lightsNew.map(Global::context);
        for (auto & info : RenderQueue::get().getQueue<LightRenderInfo>())
        {
            Light light = {};
            light.range = info->range;
            light.intensity = info->intensity;
            light.color = DirectX::SimpleMath::Vector3(info->color.x, info->color.y, info->color.z);
            light.positionVS = DirectX::SimpleMath::Vector4::Transform(
                DirectX::SimpleMath::Vector4(info->position.x, info->position.y, info->position.z, 1.f), 
                Global::mainCamera->getView()
            );

            *lightBuffer++ = light;
        }
        lightsNew.unmap(Global::context);
    }


	void Renderer::registerDebugFunction()
	{
		DebugWindow *debugWindow = DebugWindow::getInstance();
		debugWindow->registerCommand("GFX_DISABLE_POST_EFFECTS", [&](std::vector<std::string> &args)->std::string
		{
			
            enableSSAO = false;
            enableGlow = false;
            enableFog = false;
            enableDOF = false;
            enableSnow = false;

			return "Post effects off!";
		});

        debugWindow->registerCommand("GFX_ENABLE_POST_EFFECTS", [&](std::vector<std::string> &args)->std::string
        {

            enableSSAO = true;
            enableGlow = true;
            enableFog = true;
            enableDOF = true;
            enableSnow = true;

            return "Post effects on!";
        });

		debugWindow->registerCommand("GFX_SET_SSAO", [&](std::vector<std::string> &args)->std::string
		{
            std::string catcher = "";
            try
            {
                if (args.size() != 0)
                {
                    enableSSAO = std::stoi(args[0]);

                    if (enableSSAO)
                        catcher = "SSAO enabled!";

                    else
                        catcher = "SSAO disabled!";
                }
                else
                {
                    catcher = "missing argument 0 or 1.";
                }
            }
            catch (const std::exception&)
            {
                catcher = "Argument must be 0 or 1.";
            }

            return catcher;
		});

        debugWindow->registerCommand("GFX_SET_SNOW", [&](std::vector<std::string> &args)->std::string
        {
            std::string catcher = "";
            try
            {
                if (args.size() != 0)
                {
                    enableSnow = std::stoi(args[0]);

                    if (enableSnow)
                        catcher = "Snow enabled!";

                    else
                        catcher = "Snow disabled!";
                }
                else
                {
                    catcher = "missing argument 0 or 1.";
                }
            }
            catch (const std::exception&)
            {
                catcher = "Argument must be 0 or 1.";
            }

            return catcher;
        });

		debugWindow->registerCommand("GFX_SET_GLOW", [&](std::vector<std::string> &args)->std::string
		{
            std::string catcher = "";
            try
            {
                if (args.size() != 0)
                {
                    enableGlow = std::stoi(args[0]);

                    if (enableGlow)
                        catcher = "Glow enabled!";

                    else
                        catcher = "Glow disabled!";
                }
                else
                {
                    catcher = "missing argument 0 or 1.";
                }
            }
            catch (const std::exception&)
            {
                catcher = "Argument must be 0 or 1.";
            }

            return catcher;
		});

		debugWindow->registerCommand("GFX_SET_FOG", [&](std::vector<std::string> &args)->std::string
		{
            std::string catcher = "";
            try
            {
                if (args.size() != 0)
                {
                    enableFog = std::stoi(args[0]);

                    if (enableFog)
                        catcher = "Fog enabled!";

                    else
                        catcher = "Fog disabled!";
                }
                else
                {
                    catcher = "missing argument 0 or 1.";
                }
            }
            catch (const std::exception&)
            {
                catcher = "Argument must be 0 or 1.";
            }

            return catcher;
		});

		debugWindow->registerCommand("GFX_SET_DOF", [&](std::vector<std::string> &args)->std::string
		{
            std::string catcher = "";
            try
            {
                if (args.size() != 0)
                {
                    enableDOF = std::stoi(args[0]);

                    if (enableDOF)
                        catcher = "Depth of field enabled!";

                    else
                        catcher = "Depth of field disabled!";
                }
                else
                {
                    catcher = "missing argument 0 or 1.";
                }
            }
            catch (const std::exception&)
            {
                catcher = "Argument must be 0 or 1.";
            }

            return catcher;
		});

        debugWindow->registerCommand("GFX_SET_DOF_SLIDERS", [&](std::vector<std::string> &args)->std::string
        {
            std::string catcher = "";
            try
            {
                if (args.size() != 0)
                {
                    enableCoCWindow = std::stoi(args[0]);

                    if (enableCoCWindow)
                        catcher = "Circle of confusion window enabled!";

                    else
                        catcher = "Circle of confusion window disabled!";
                }
                else
                {
                    catcher = "missing argument 0 or 1.";
                }
            }
            catch (const std::exception&)
            {
                catcher = "Argument must be 0 or 1.";
            }

            return catcher;
        });

        debugWindow->registerCommand("GFX_SET_HUD", [&](std::vector<std::string> &args)->std::string
        {
            std::string catcher = "";
            try
            {
                if (args.size() != 0)
                {
                    enableHud = std::stoi(args[0]);

                    if (enableHud)
                        catcher = "HUD enabled!";

                    else
                        catcher = "HUD disabled!";
                }
                else
                {
                    catcher = "missing argument 0 or 1.";
                }
            }
            catch (const std::exception&)
            {
                catcher = "Argument must be 0 or 1.";
            }

            return catcher;
        });

		debugWindow->registerCommand("GFX_SET_FREEZE", [&](std::vector<std::string> &args)->std::string
		{
			std::string catcher = "";
			try
			{
                if (args.size() != 0)
                {
                    this->statusData.freeze = std::stof(args[0]);
                }
                else
                {
                    return "missing argument freeze amount";
                }
			}
			catch (const std::exception&)
			{
				catcher = "Argument must be float between 1 and 0";
			}

			return catcher;
		});

		debugWindow->registerCommand("GFX_SET_BURN", [&](std::vector<std::string> &args)->std::string
		{
			std::string catcher = "";
			try
			{
                if (args.size() != 0)
                {
                    this->statusData.burn = std::stof(args[0]);
                }
                else
                {
                    return "missing argument burn amount";
                }
			}
			catch (const std::exception&)
			{
				catcher = "Argument must be float between 1 and 0";
			}

			return catcher;
		});

		debugWindow->registerCommand("GFX_RELOAD_FORWARD_SHADER", [&](std::vector<std::string> &args)->std::string
		{
			std::string catcher = "";
			
			forwardPlus.recompile(Global::device, SHADER_PATH("ForwardPlus.hlsl"), VERTEX_DESC);


			return catcher;
		});

		debugWindow->registerCommand("GFX_RELOAD_GLOW_SHADERS", [&](std::vector<std::string> &args)->std::string
		{
			std::string catcher = "";

			glowRenderer.recompileGlow(Global::device);

			return catcher;
		});

		/*debugWindow->registerCommand("GFX_RELOAD_SNOW_SHADER", [&](std::vector<std::string> &args)->std::string
		{
			std::string catcher = "";

			snowManager.recompile(Global::device);

			return catcher;
		});*/
	}
}
