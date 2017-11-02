#include <Graphics\include\Renderer.h>
#include <stdio.h>
#include <Graphics\include\ThrowIfFailed.h>
#include <Engine\Constants.h>
#include <Keyboard.h>
#include <Engine\DebugWindow.h>

#include <Engine\Profiler.h>

#include "Particles\ParticleSystem.h"


#define USE_TEMP_CUBE false
#define ANIMATION_HIJACK_RENDER false

#if USE_TEMP_CUBE
#include "TempCube.h"
#endif
#if ANIMATION_HIJACK_RENDER
#include "Animation\AnimatedTestCube.h"
#endif

#define MAX_DEBUG_POINTS 10000

namespace Graphics
{

	Renderer::Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera)
		: forwardPlus(device, SHADER_PATH("ForwardPlus.hlsl"), VERTEX_DESC)
		, fullscreenQuad(device, SHADER_PATH("FullscreenQuad.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R8_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } })
		, depthStencil(device, WIN_WIDTH, WIN_HEIGHT)
		, instanceSBuffer(device, CpuAccess::Write, INSTANCE_CAP)
		, instanceOffsetBuffer(device)
		, skyRenderer(device, SHADOW_MAP_RESOLUTION)
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
		, snowManager(device)


#pragma endregion
		, depthShader(device, SHADER_PATH("DepthPixelShader.hlsl"), {}, ShaderType::PS)
	{
		this->device = device;
		this->deviceContext = deviceContext;
		this->backBuffer = backBuffer;

		initialize(device, deviceContext, camera);

		fakeBackBuffer = newd ShaderResource(device, WIN_WIDTH, WIN_HEIGHT);
		fakeBackBufferSwap = newd ShaderResource(device, WIN_WIDTH, WIN_HEIGHT);

		viewPort = { 0 };
		viewPort.Width = WIN_WIDTH;
		viewPort.Height = WIN_HEIGHT;
		viewPort.MaxDepth = 1.0f;

        states = newd DirectX::CommonStates(device);
        grid.initialize(camera, device, deviceContext, &resourceManager);

        FXSystem = newd ParticleSystem(device, 512, "Resources/Particles/base.part");

        float temp = 1.f;
        bulletTimeBuffer.write(deviceContext, &temp, sizeof(float));

		//menuSprite = std::make_unique<DirectX::SpriteBatch>(deviceContext);
		createBlendState();

		
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
        resourceManager.release();

    }

    void Renderer::initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext, Camera * camera)
    {
        resourceManager.initialize(gDevice, gDeviceContext);
		skyRenderer.initialize(resourceManager.getModelInfo(SKY_SPHERE));

        //temp
        DirectX::CreateWICTextureFromFile(device, TEXTURE_PATH("glowMapTree.png"), NULL, &glowTest);
		snowManager.initializeSnowflakes(camera);
    }

	void Renderer::updateLight(float deltaTime, Camera * camera)
	{
		PROFILE_BEGIN("UpdateLights()");
		skyRenderer.update(deviceContext, deltaTime, camera->getPos());
		PROFILE_END();

		//Temp or rename function
		PROFILE_BEGIN("updateSnow()");
		snowManager.updateSnow(deltaTime, camera, deviceContext);
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
    {
        menu.unloadTextures();
#if ANIMATION_HIJACK_RENDER

        renderQueue.clear();
        static Camera cam(device, WIN_WIDTH, WIN_HEIGHT);
        static UINT ticks = 0;
        ticks++;
        cam.updateLookAt({ 5 * sinf(ticks * 0.001f), 5 * cosf(ticks * 0.001f), 5 }, { 0,0,0 }, deviceContext);

        ID3D11Buffer *cameraBuffer = cam.getBuffer();
        deviceContext->VSSetConstantBuffers(0, 1, &cameraBuffer);
        deviceContext->PSSetConstantBuffers(0, 1, &cameraBuffer);

        static float clearColor[4] = { 0,0,0,1 };
        deviceContext->ClearRenderTargetView(backBuffer, clearColor);
        deviceContext->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.f, 0);

        deviceContext->RSSetViewports(1, &viewPort);

        static AnimatedTestCube testCube(device);
        static Shader testShader(device, SHADER_PATH("AnimationTest.hlsl"), ANIMATED_VERTEX_DESC);

        deviceContext->IASetInputLayout(testShader);
        deviceContext->VSSetShader(testShader, nullptr, 0);
        deviceContext->PSSetShader(testShader, nullptr, 0);

        deviceContext->IASetVertexBuffers(0, 1, testCube, &testCube.stride, &testCube.offset);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->RSSetState(states->CullNone());

        deviceContext->OMSetRenderTargets(1, &backBuffer, depthStencil);

        deviceContext->Draw(testCube.vertexCount, 0);

        auto jointTransforms = testSkeleton.getJointTransforms(testAnimation, testAnimation.getTotalDuration() * ((ticks % 1000) / 1000.f));

        static StructuredBuffer<Matrix> jointBuffer(device, CpuAccess::Write, testSkeleton.getJointCount());
        Matrix* bufferPtr = jointBuffer.map(deviceContext);
        memcpy(bufferPtr, jointTransforms.data(), sizeof(Matrix) * jointTransforms.size());
        jointBuffer.unmap(deviceContext);

        ID3D11ShaderResourceView * jointView = jointBuffer.getSRV();
        deviceContext->VSSetShaderResources(0, 1, &jointView);

#else

		PROFILE_BEGIN("clear()");
		clear();
		PROFILE_END();

		PROFILE_BEGIN("Cull()");
		cull();
		PROFILE_END();

		PROFILE_BEGIN("WriteInstanceData()");
		writeInstanceData();
		PROFILE_END();

		PROFILE_BEGIN("drawShadows()");

		deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);
		//Drawshadows does not actually draw anything, it just sets up everything for drawing shadows
		skyRenderer.drawShadows(deviceContext, &forwardPlus);
		draw();
		PROFILE_END();


		PROFILE_BEGIN("depthPass");

		deviceContext->PSSetConstantBuffers(0, 1, *camera->getBuffer());
		deviceContext->VSSetConstantBuffers(0, 1, *camera->getBuffer());

		


		deviceContext->RSSetViewports(1, &viewPort);
		deviceContext->RSSetState(states->CullCounterClockwise());

        deviceContext->IASetInputLayout(forwardPlus);
        deviceContext->VSSetShader(forwardPlus, nullptr, 0);

        deviceContext->PSSetShader(nullptr, nullptr, 0);
        deviceContext->OMSetRenderTargets(0, nullptr, depthStencil);

        draw();

		deviceContext->IASetInputLayout(foliageShader);
		deviceContext->VSSetShader(foliageShader, nullptr, 0);
		deviceContext->PSSetShader(depthShader, nullptr, 0);
		
		//this be no deltatime
		grassTime++;

		drawFoliage(camera);
		PROFILE_END();

		PROFILE_BEGIN("grid.updateLights()");
		deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		deviceContext->RSSetState(states->CullCounterClockwise());

		grid.updateLights(deviceContext, camera, lights);
		lights.clear();
		PROFILE_END();

		PROFILE_BEGIN("grid.cull()");
		grid.cull(camera, states, depthStencil, device, deviceContext, &resourceManager);
		PROFILE_END();

        PROFILE_BEGIN("FXSystem::update()")
        FXSystem->update(deviceContext, camera, 0.016f);
        PROFILE_END();

        PROFILE_BEGIN("FXSystem::render()")
        FXSystem->render(deviceContext, camera, states, *fakeBackBuffer, depthStencil, false);
        PROFILE_END();

		PROFILE_BEGIN("draw()");
		deviceContext->IASetInputLayout(forwardPlus);
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
		
		draw();
		PROFILE_END();

		PROFILE_BEGIN("RenderFoliage");
		deviceContext->IASetInputLayout(foliageShader);
		deviceContext->VSSetShader(foliageShader, nullptr, 0);
		deviceContext->PSSetShader(foliageShader, nullptr, 0);
		drawFoliage(camera);
		renderFoliageQueue.clear();
		PROFILE_END();
		

	/*	PROFILE_BEGIN("RenderWater");
		drawWater(camera);
		PROFILE_END();*/

		deviceContext->PSSetConstantBuffers(2, 1, bulletTimeBuffer);
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


#endif
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
            hud.drawHUD(deviceContext, backBuffer, transparencyBlendState);
            PROFILE_END();
        }

		PROFILE_BEGIN("DebugInfo");
		renderDebugInfo(camera);
		PROFILE_END();
	}


    void Renderer::queueRender(RenderInfo * renderInfo)
    {
        if (renderQueue.size() > INSTANCE_CAP)
            throw "Renderer Exceeded Instance Cap.";

        renderQueue.push_back(renderInfo);
    }

	void Renderer::queueFoliageRender(FoliageRenderInfo * renderInfo)
	{
		if (renderFoliageQueue.size() > INSTANCE_CAP)
		{
			throw "Foliage renderer exceeded instance cap.";
		}

		renderFoliageQueue.push_back(renderInfo);
	
	}

	void Renderer::queueWaterRender(WaterRenderInfo * renderInfo)
	{
		if (renderWaterQueue.size() > INSTANCE_CAP)
		{
			throw "Water renderer exceeded instance cap.";
		}

		renderWaterQueue.push_back(renderInfo);
	}

    void Renderer::queueRenderDebug(RenderDebugInfo * debugInfo)
    {
        if (debugInfo->points->size() > MAX_DEBUG_POINTS)
        {
            throw "vector is bigger than structured buffer";
        }
        renderDebugQueue.push_back(debugInfo);
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
			ModelInfo model = resourceManager.getModelInfo(info->meshId);

			static UINT stride = sizeof(Vertex), offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, &model.vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(model.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			static ID3D11ShaderResourceView * modelTextures[1] = { nullptr };
			modelTextures[0] = model.diffuseMap;
			deviceContext->PSSetShaderResources(10, 1, modelTextures);

			PROFILE_BEGIN("DrawIndexed()");
			deviceContext->DrawIndexed((UINT)model.indexCount, 0, 0);
			PROFILE_END();
		}

	}

    void Renderer::draw()
    {
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->VSSetConstantBuffers(4, 1, instanceOffsetBuffer);
        deviceContext->VSSetShaderResources(20, 1, instanceSBuffer);


        UINT instanceOffset = 0;
        for (InstanceQueue_t::value_type & pair : instanceQueue)
        {
			PROFILE_BEGIN("Setup for draw");
            instanceOffsetBuffer.write(deviceContext, &instanceOffset, sizeof(UINT));
            instanceOffset += (UINT)pair.second.size();

            ModelInfo model = resourceManager.getModelInfo(pair.first);

            static UINT stride = sizeof(Vertex), offset = 0;
            deviceContext->IASetVertexBuffers(0, 1, &model.vertexBuffer, &stride, &offset);
            deviceContext->IASetIndexBuffer(model.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

            static ID3D11ShaderResourceView * modelTextures[4] = { nullptr };
            modelTextures[0] = model.diffuseMap;
            modelTextures[1] = model.normalMap;
            modelTextures[2] = model.specularMap;
            modelTextures[3] = model.glowMap;
            deviceContext->PSSetShaderResources(10, 4, modelTextures);
			PROFILE_END();

			PROFILE_BEGIN("DrawIndexedInstanced()");
            deviceContext->DrawIndexedInstanced((UINT)model.indexCount, (UINT)pair.second.size(), 0, 0, 0);
			PROFILE_END();
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
        hud.renderText(transparencyBlendState, false);

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
                (UINT)(info->points->size() * sizeof(DirectX::SimpleMath::Vector3))
            );

            debugColorBuffer.write(
                deviceContext,
                &info->color,
                (UINT)sizeof(DirectX::SimpleMath::Color)
            );

            deviceContext->IASetPrimitiveTopology(info->topology);
            deviceContext->OMSetDepthStencilState(info->useDepth ? states->DepthDefault() : states->DepthNone(), 0);
            deviceContext->Draw((UINT)info->points->size(), 0);
        }

        renderDebugQueue.clear();
    }


    void Renderer::createBlendState()
    {
        D3D11_BLEND_DESC BlendState;
        ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
        BlendState.RenderTarget[0].BlendEnable = TRUE;
        BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;

        ThrowIfFailed(this->device->CreateBlendState(&BlendState, &transparencyBlendState));
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

		debugWindow->registerCommand("GFX_TOGGLE_SSAO", [&](std::vector<std::string> &args)->std::string
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

        debugWindow->registerCommand("GFX_TOGGLE_SNOW", [&](std::vector<std::string> &args)->std::string
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

		debugWindow->registerCommand("GFX_TOGGLE_GLOW", [&](std::vector<std::string> &args)->std::string
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

		debugWindow->registerCommand("GFX_TOGGLE_FOG", [&](std::vector<std::string> &args)->std::string
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

		debugWindow->registerCommand("GFX_TOGGLE_DOF", [&](std::vector<std::string> &args)->std::string
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

        debugWindow->registerCommand("GFX_TOGGLE_DOF_SLIDERS", [&](std::vector<std::string> &args)->std::string
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

        debugWindow->registerCommand("GFX_TOGGLE_HUD", [&](std::vector<std::string> &args)->std::string
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
			
			forwardPlus.recompile(device, SHADER_PATH("ForwardPlus.hlsl"), VERTEX_DESC);


			return catcher;
		});

		debugWindow->registerCommand("GFX_RELOAD_GLOW_SHADERS", [&](std::vector<std::string> &args)->std::string
		{
			std::string catcher = "";

			glowRenderer.recompileGlow(device);

			return catcher;
		});

		debugWindow->registerCommand("GFX_RELOAD_SNOW_SHADER", [&](std::vector<std::string> &args)->std::string
		{
			std::string catcher = "";

			snowManager.recompile(device);

			return catcher;
		});
	}
}
