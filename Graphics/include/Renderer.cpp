#include <Graphics\include\Renderer.h>
#include <stdio.h>
#include <Graphics\include\ThrowIfFailed.h>
#include <Engine\Constants.h>
#include "Utility\sizeofv.h"
#include <Keyboard.h>
#include <Engine\DebugWindow.h>

#include <Engine\Profiler.h>
#include "RenderQueue.h";

#include "Particles\ParticleSystem.h"


#define USE_TEMP_CUBE false
#define ANIMATION_HIJACK_RENDER false

#if USE_TEMP_CUBE
#include "TempCube.h"
#endif


#define MAX_DEBUG_POINTS 10000
#define _INSTANCE_CAP 300

namespace Graphics
{

	Renderer::Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera)
		: forwardPlus(device, Resources::Shaders::ForwardPlus)
        , hybrisLoader(device)
		, fullscreenQuad(device, SHADER_PATH("FullscreenQuad.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R8_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } })
		, depthStencil(device, WIN_WIDTH, WIN_HEIGHT)
		, instanceSBuffer(device, CpuAccess::Write, _INSTANCE_CAP)
		, instanceOffsetBuffer(device)
		, skyRenderer(device, SHADOW_MAP_RESOLUTION, hybrisLoader)
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
        , staticInstanceBuffer(device, CpuAccess::Write, StaticRenderInfo::INSTANCE_CAP)
	{
		this->device = device;
		this->deviceContext = deviceContext;
		this->backBuffer = backBuffer;

		initialize(device, deviceContext);

		fakeBackBuffer = newd ShaderResource(device, WIN_WIDTH, WIN_HEIGHT);
		fakeBackBufferSwap = newd ShaderResource(device, WIN_WIDTH, WIN_HEIGHT);

		viewPort = { 0 };
		viewPort.Width = WIN_WIDTH;
		viewPort.Height = WIN_HEIGHT;
		viewPort.MaxDepth = 1.0f;

		states = newd DirectX::CommonStates(device);
		grid.initialize(camera, device, deviceContext);

        FXSystem = newd ParticleSystem(device, 512, "Resources/Particles/base.part");

        float temp = 1.f;
        bulletTimeBuffer.write(deviceContext, &temp, sizeof(float));

		//menuSprite = std::make_unique<DirectX::SpriteBatch>(deviceContext);
		createBlendState();

		DebugWindow *debugWindow = DebugWindow::getInstance();
		debugWindow->registerCommand("TOGGLEPOSTEFFECTS", [&](std::vector<std::string> &args)->std::string
		{
			enableSSAO = false;
			enableGlow= false;
			enableDOF = false;
			enableFog = false;

			return "PostEffect OFF";
		});
		registerDebugFunction();

		statusData.burn = 0;
		statusData.freeze = 0;
    }


    Renderer::~Renderer()
    {
		delete states;
		delete fakeBackBuffer;
		delete fakeBackBufferSwap;
        delete FXSystem;
		SAFE_RELEASE(transparencyBlendState);

		SAFE_RELEASE(glowTest);
//        resourceManager.release();

    }

    void Renderer::initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext)
    {
        //resourceManager.initialize(gDevice, gDeviceContext);
		//skyRenderer.initialize(resourceManager.getModelInfo(SKY_SPHERE));

        //temp
        DirectX::CreateWICTextureFromFile(device, TEXTURE_PATH("glowMapTree.png"), NULL, &glowTest);

    }

	void Renderer::updateLight(float deltaTime, Camera * camera)
	{
		PROFILE_BEGIN("UpdateLights()");
		skyRenderer.update(deviceContext, deltaTime, camera->getPos());
		PROFILE_END();
	}

	//this function is called in SkillBulletTime.cpp
	void Renderer::setBulletTimeCBuffer(float amount)
	{
		PROFILE_BEGIN("SetBulletTimeCBuffer()");
		//These two must always add up to one ir i'll have to fix the formula
		//They represents how long the fade in and fade out are. 
		static const float TOP_THRESHOLD = 0.9;
		static const float BOT_THRESHOLD = 0.1;


        if (amount > TOP_THRESHOLD)
            amount = (amount - TOP_THRESHOLD) / BOT_THRESHOLD;

        else if (amount < BOT_THRESHOLD)
            amount = 1 - ((amount) / BOT_THRESHOLD);

        else amount = 0;

        bulletTimeBuffer.write(deviceContext, &amount, sizeof(float));
        PROFILE_END();
    }

	void Renderer::updateShake(float deltaTime)
	{
		PROFILE_BEGIN("UpdateShake()");
		hud.updateShake(deviceContext, deltaTime);
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

        RenderQueue_t::get().clearAllQueues();
    }
    #else
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

		////drawFoliage(camera);
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

        PROFILE_BEGIN("FXSystem::update()")
        FXSystem->update(deviceContext, camera, 0.016f);
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

		ID3D11SamplerState * samplers[] = { skyRenderer.getSampler() };
		deviceContext->PSSetSamplers(1, 1, samplers);

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

		PROFILE_BEGIN("DebugThings");
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

		deviceContext->RSSetState(states->CullCounterClockwise());
		SHORT tabKeyState = GetAsyncKeyState(VK_TAB);
		if ((1 << 15) & tabKeyState)
		{
			this->drawToBackbuffer(grid.getDebugSRV());
		}
		PROFILE_END();

		//TEEEMP
		auto ks = DirectX::Keyboard::Get().GetState();
		
		
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
		}

		else
		{
			PROFILE_BEGIN("DrawToBackBuffer");
			drawToBackbuffer(*fakeBackBuffer);
			PROFILE_END();
		}



        PROFILE_BEGIN("HUD");
        hud.drawHUD(deviceContext, backBuffer, transparencyBlendState);
        PROFILE_END();

		PROFILE_BEGIN("DebugInfo");
		renderDebugInfo(camera);
		PROFILE_END();

		if (ks.G)
		{
			startShake(30, 1000);
		}

        RenderQueue_t::get().clearAllQueues();
	}
    #endif

    void Renderer::queueRender(RenderInfo * renderInfo)
    {
    }

	void Renderer::queueFoliageRender(FoliageRenderInfo * renderInfo)
	{
		//if (renderFoliageQueue.size() > INSTANCE_CAP)
		//{
		//	throw "Foliage renderer exceeded instance cap.";
		//}

		////renderFoliageQueue.push_back(renderInfo);
	
	}

	void Renderer::queueWaterRender(WaterRenderInfo * renderInfo)
	{
		//if (renderWaterQueue.size() > INSTANCE_CAP)
		//{
		//	throw "Water renderer exceeded instance cap.";
		//}

		//renderWaterQueue.push_back(renderInfo);
	}

    void Renderer::queueRenderDebug(RenderDebugInfo * debugInfo)
    {
        //if (debugInfo->points->size() > MAX_DEBUG_POINTS)
        //{
        //    throw "vector is bigger than structured buffer";
        //}
        ////renderDebugQueue.push_back(debugInfo);
    }

    void Renderer::queueText(TextString * text)
    {
        hud.queueText(text);
    }

	void Renderer::queueLight(Light light)
	{
		lights.push_back(light);
	}

    void Renderer::fillHUDInfo(HUDInfo * info)
    {
        hud.fillHUDInfo(info);
    }

    void Renderer::cull()
    {
        instanceQueue.clear();
        for (RenderInfo * info : renderQueue)
        {
            if (info->render)
            {
                instanceQueue[info->meshId].push_back({ 
					info->translation,
					info->translation.Invert().Transpose(),
					this->statusData.freeze,
					this->statusData.burn
				});
            }
        }
        renderQueue.clear();
    }

    void Renderer::writeInstanceData()
    {
        InstanceData* ptr = instanceSBuffer.map(deviceContext);
        for (InstanceQueue_t::value_type & pair : instanceQueue)
        {
            void * data = pair.second.data();
            size_t size = pair.second.size() * sizeof(InstanceData);
            memcpy(ptr, data, size);
            ptr = (InstanceData*)((char*)ptr + size);
        }
        instanceSBuffer.unmap(deviceContext);
    }

	void Renderer::drawFoliage(Camera * camera)
	{
		timeBuffer.write(deviceContext, &grassTime, sizeof(grassTime));

		deviceContext->VSSetConstantBuffers(4, 1, timeBuffer);
		deviceContext->RSSetState(states->CullNone());
		deviceContext->OMSetRenderTargets(1, *fakeBackBuffer, depthStencil);

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		UINT sampleMask = 0xffffffff;
		deviceContext->OMSetBlendState(transparencyBlendState, blendFactor, sampleMask);

		for (FoliageRenderInfo * info : renderFoliageQueue)
		{
			/*ModelInfo model = resourceManager.getModelInfo(info->meshId);

			static UINT stride = sizeof(Vertex), offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, &model.vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(model.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			static ID3D11ShaderResourceView * modelTextures[1] = { nullptr };
			modelTextures[0] = model.diffuseMap;
			deviceContext->PSSetShaderResources(10, 1, modelTextures);

			PROFILE_BEGIN("DrawIndexed()");
			deviceContext->DrawIndexed((UINT)model.indexCount, 0, 0);
			PROFILE_END();*/
		}

	}

    void Renderer::drawStatic()
    {
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->RSSetState(states->CullClockwise());
        deviceContext->VSSetShaderResources(5, 1, staticInstanceBuffer);

        UINT instanceOffset = 0;
        for (auto & pair : RenderQueue_t::get().getQueue<StaticRenderInfo>())
        {
            auto & modelId = pair.first;
            auto & renderInfos = pair.second;

            HybrisLoader::Model    * model = hybrisLoader.getModel(modelId);
            HybrisLoader::Mesh     * mesh = &model->getMesh();
            HybrisLoader::Material * material = &model->getMaterial();
            HybrisLoader::Skeleton * skeleton = &model->getSkeleton();

            deviceContext->VSSetShaderResources(4, 1, mesh->getVertexBuffer());

            ID3D11ShaderResourceView * textures[4] =
            {
                /*Diffuse */ material->getDiffuse(),
                /*Normal  */ material->getNormals(),
                /*Specular*/ material->getSpecular(),
                /*Glow    */ material->getGlow()
            };
            deviceContext->PSSetShaderResources(10, 4, textures);

            deviceContext->DrawInstanced(
                mesh->getVertexCount(), 
                renderInfos.size(),
                0,
                instanceOffset
            );

            instanceOffset += renderInfos.size();
        }
    }

	void Renderer::clear()
	{
		static float clearColor[4] = { 0 };
		deviceContext->ClearRenderTargetView(backBuffer, clearColor);
		deviceContext->ClearRenderTargetView(*fakeBackBuffer, clearColor);
		deviceContext->ClearRenderTargetView(glowRenderer, clearColor);
		deviceContext->ClearRenderTargetView(backBuffer, clearColor);
		deviceContext->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.f, 0);
		skyRenderer.clear(deviceContext);
		glowRenderer.clear(deviceContext, clearColor);
	}

	void Renderer::swapBackBuffers()
	{
		ShaderResource * temp = fakeBackBuffer;
		fakeBackBuffer = fakeBackBufferSwap;
		fakeBackBufferSwap = temp;
	}

    void Renderer::drawToBackbuffer(ID3D11ShaderResourceView * texture)
    {
        deviceContext->PSSetShaderResources(0, 1, &texture);

        UINT zero = 0;
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);

        deviceContext->IASetInputLayout(fullscreenQuad);
        deviceContext->VSSetShader(fullscreenQuad, nullptr, 0);
        deviceContext->PSSetShader(fullscreenQuad, nullptr, 0);

        static ID3D11SamplerState * pointClamp = states->PointClamp();
        deviceContext->PSSetSamplers(0, 1, &pointClamp);

		PROFILE_BEGIN("Draw(4, 0)");
        deviceContext->Draw(4, 0);
		PROFILE_END();

        ID3D11ShaderResourceView * srvNull = nullptr;
        deviceContext->PSSetShaderResources(0, 1, &srvNull);
    }

    //draws the menu
    void Renderer::drawMenu(Graphics::MenuInfo * info)
    {
        deviceContext->RSSetViewports(1, &viewPort);
        menu.drawMenu(device, deviceContext, info, backBuffer, transparencyBlendState);
        hud.renderText(transparencyBlendState);

    }


    void Renderer::renderDebugInfo(Camera* camera)
    {
        if (renderDebugQueue.size() == 0) return;

		deviceContext->PSSetConstantBuffers(0, 1, *camera->getBuffer());
		deviceContext->VSSetConstantBuffers(0, 1, *camera->getBuffer());

        deviceContext->OMSetRenderTargets(1, &backBuffer, depthStencil);

        deviceContext->VSSetShaderResources(0, 1, debugPointsBuffer);
        deviceContext->PSSetConstantBuffers(1, 1, debugColorBuffer);

        deviceContext->IASetInputLayout(nullptr);
        deviceContext->VSSetShader(debugRender, nullptr, 0);
        deviceContext->PSSetShader(debugRender, nullptr, 0);


        for (RenderDebugInfo * info : renderDebugQueue)
        {
            debugPointsBuffer.write(
                deviceContext,
                info->points->data(),
                info->points->size() * sizeof(DirectX::SimpleMath::Vector3)
            );

            debugColorBuffer.write(
                deviceContext,
                &info->color,
                sizeof(DirectX::SimpleMath::Color)
            );

            deviceContext->IASetPrimitiveTopology(info->topology);
            deviceContext->OMSetDepthStencilState(info->useDepth ? states->DepthDefault() : states->DepthNone(), 0);
            deviceContext->Draw(info->points->size(), 0);
        }

        renderDebugQueue.clear();
    }

    void Renderer::writeInstanceBuffers()
    {
        void * dest = staticInstanceBuffer.map(deviceContext);
        size_t offset = 0;

        for (auto & model_infos : RenderQueue_t::get().getQueue<StaticRenderInfo>())
        {
            for (auto & info : model_infos.second)
            {
                InstanceData instanceData = {};
                instanceData.transform = info->transform;
                instanceData.transformInvT = info->transform.Invert().Transpose();

                memcpy((char*)dest + offset, &instanceData, sizeof(instanceData));
                offset += sizeof(instanceData);
            }
        }
        staticInstanceBuffer.unmap(deviceContext);
    }

	
    void Renderer::createBlendState()
    {
        D3D11_BLEND_DESC BlendState;
        ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
        BlendState.RenderTarget[0].BlendEnable = TRUE;
        BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
        BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;

        ThrowIfFailed(this->device->CreateBlendState(&BlendState, &transparencyBlendState));
    }

















	void Renderer::registerDebugFunction()
	{
		DebugWindow *debugWindow = DebugWindow::getInstance();
		debugWindow->registerCommand("TOGGLEPOSTEFFECTS", [&](std::vector<std::string> &args)->std::string
		{
			enablePostEffects = !enablePostEffects;

			return "Post effects toggled!";
		});

		debugWindow->registerCommand("TOGGLESSAO", [&](std::vector<std::string> &args)->std::string
		{
			enableSSAO = !enableSSAO;

			return "Post effects toggled!";
		});

		debugWindow->registerCommand("TOGGLEGLOW", [&](std::vector<std::string> &args)->std::string
		{
			enableGlow = !enableGlow;

			return "Post effects toggled!";
		});

		debugWindow->registerCommand("TOGGLEFOG", [&](std::vector<std::string> &args)->std::string
		{
			enableFog = !enableFog;

			return "Post effects toggled!";
		});

		debugWindow->registerCommand("TOGGLEDOF", [&](std::vector<std::string> &args)->std::string
		{
			enableDOF = !enableDOF;

			return "Post effects toggled!";
		});

		debugWindow->registerCommand("SETFREEZE", [&](std::vector<std::string> &args)->std::string
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

		debugWindow->registerCommand("SETBURN", [&](std::vector<std::string> &args)->std::string
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

		debugWindow->registerCommand("RELOADFORWARDSHADER", [&](std::vector<std::string> &args)->std::string
		{
			std::string catcher = "";
			
			forwardPlus.recompile(device, SHADER_PATH("ForwardPlus.hlsl"), VERTEX_DESC);


			return catcher;
		});

		debugWindow->registerCommand("RELOADGLOWSHADERS", [&](std::vector<std::string> &args)->std::string
		{
			std::string catcher = "";

			glowRenderer.recompileGlow(device);

			return catcher;
		});
        debugWindow->registerCommand("ENABLEDOFSLIDERS", [&](std::vector<std::string> &args)->std::string
        {
            enableCoCWindow = !enableCoCWindow;

            return "Post effects toggled!";
        });
	}
}
