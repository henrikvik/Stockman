#include <Graphics\include\Renderer.h>
#include <stdio.h>
#include <Graphics\include\ThrowIfFailed.h>
#include <Engine\Constants.h>

#include <Engine\Profiler.h>

#define USE_TEMP_CUBE false
#define ANIMATION_HIJACK_RENDER false

#if USE_TEMP_CUBE
#include "TempCube.h"
#endif
#if ANIMATION_HIJACK_RENDER
#include "Animation\AnimatedTestCube.h"
#endif

#define MAX_DEBUG_POINTS 100

namespace Graphics
{

	Renderer::Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera)
		: forwardPlus(device, SHADER_PATH("ForwardPlus.hlsl"), VERTEX_DESC)
		, fullscreenQuad(device, SHADER_PATH("FullscreenQuad.hlsl"), { { "POSITION", 0, DXGI_FORMAT_R8_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } })
        , menuShader(device, SHADER_PATH("MenuShader.hlsl"), { {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA}, {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA} })
        , GUIShader(device, SHADER_PATH("GUIShader.hlsl"), { {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA } , {"ELEMENT", 0, DXGI_FORMAT_R32_UINT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA} })
		, depthStencil(device, WIN_WIDTH, WIN_HEIGHT)
        , instanceSBuffer(device, CpuAccess::Write, INSTANCE_CAP)
        , instanceOffsetBuffer(device)
		, skyRenderer(device, SHADOW_MAP_RESOLUTION)
		, postProcessor(device, deviceContext)
		, fakeBackBuffer(device, WIN_WIDTH, WIN_HEIGHT)
		, fakeBackBufferSwap(device, WIN_WIDTH, WIN_HEIGHT)
		, glowMap(device, WIN_WIDTH, WIN_HEIGHT)
    #pragma region RenderDebugInfo
        , debugPointsBuffer(device, CpuAccess::Write, MAX_DEBUG_POINTS)
        , debugRender(device, SHADER_PATH("DebugRender.hlsl"))
        , debugColorBuffer(device)
    #pragma endregion

	{
		this->device = device;
		this->deviceContext = deviceContext;
		this->backBuffer = backBuffer;
		
		initialize(device, deviceContext);

        viewPort = { 0 };
        viewPort.Width = WIN_WIDTH;
        viewPort.Height = WIN_HEIGHT;
        viewPort.MaxDepth = 1.0f;

		states = new DirectX::CommonStates(device);
		grid.initialize(camera, device, deviceContext, &resourceManager);

        //menuSprite = std::make_unique<DirectX::SpriteBatch>(deviceContext);
        loadModellessTextures();
        menuTexturesLoaded = true;
        createMenuVBS();
        createGUIBuffers();
        createBlendState();
    }


    Renderer::~Renderer()
    {
		delete states;
		SAFE_RELEASE(GUIvb);
		SAFE_RELEASE(transparencyBlendState);
        SAFE_RELEASE(menuQuad);
        SAFE_RELEASE(buttonQuad);
        unloadMenuTextures();
        SAFE_RELEASE(GUITexture1);
        SAFE_RELEASE(GUITexture2);
		SAFE_RELEASE(glowTest);
        resourceManager.release();

    }

    void Renderer::initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext)
    {
        resourceManager.initialize(gDevice, gDeviceContext);

		//temp
		DirectX::CreateWICTextureFromFile(device, TEXTURE_PATH("glowMapTree.png"), NULL, &glowTest);


    }

	void Renderer::updateLight(float deltaTime, Camera * camera)
	{
		skyRenderer.update(deviceContext, deltaTime, camera->getPos());
	}

    void Renderer::render(Camera * camera)
    {
        unloadMenuTextures();
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
        cull();
        writeInstanceData();

		//Drawshadows does not actually draw anything, it just sets up everything for drawing shadows
		skyRenderer.drawShadows(deviceContext, &forwardPlus);
		draw();


		ID3D11Buffer *cameraBuffer = camera->getBuffer();
		deviceContext->VSSetConstantBuffers(0, 1, &cameraBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &cameraBuffer);

		static float clearColor[4] = { 0, 0.5, 0.7, 1 };
		static float blackClearColor[4] = {0};
		deviceContext->ClearRenderTargetView(fakeBackBuffer, clearColor);
		deviceContext->ClearRenderTargetView(glowMap, blackClearColor);
		deviceContext->ClearRenderTargetView(backBuffer, clearColor);
        deviceContext->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH, 1.f, 0);


		deviceContext->RSSetViewports(1, &viewPort);
		deviceContext->RSSetState(states->CullCounterClockwise());

        deviceContext->IASetInputLayout(forwardPlus);
        deviceContext->VSSetShader(forwardPlus, nullptr, 0);

        deviceContext->PSSetShader(nullptr, nullptr, 0);
        deviceContext->OMSetRenderTargets(0, nullptr, depthStencil);
        deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);

        draw();

        deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

    #pragma region Light Temp
        static 	float f = 59.42542;
        f += 0.001f;

        auto lights = grid.getLights();

        Light *ptr = lights->map(deviceContext);
        for (int i = 0; i < NUM_LIGHTS; i++) {
            ptr[i].color = DirectX::SimpleMath::Vector3(
                ((unsigned char)(5 + i * 53 * i + 4)) / 255.f,
                ((unsigned char)(66 + i * 23 + 4)) / 255.f,
                ((unsigned char)(11 + i * 455 + 4)) / 255.f
            );
            ptr[i].positionWS = (ptr[i].color * 2 - DirectX::SimpleMath::Vector3(1.f)) * 2;
            ptr[i].positionWS.x = sin(f) * ptr[i].positionWS.x * 8;
            ptr[i].positionWS.y = 1.f;
            ptr[i].positionWS.z = cos(f) * ptr[i].positionWS.z * 8;

            //ptr[i].positionWS = DirectX::SimpleMath::Vector3(0.f, 1.f, 1.f - 1 / 8.f - i / 4.f);
            ptr[i].positionVS = DirectX::SimpleMath::Vector4::Transform(DirectX::SimpleMath::Vector4(ptr[i].positionWS.x, ptr[i].positionWS.y, ptr[i].positionWS.z, 1.f), camera->getView());
			ptr[i].range = i / 1.f;// 1.f + ((unsigned char)(i * 53 * i + 4)) / 255.f * i;
            ptr[i].intensity = 1.f;
        }

        lights->unmap(deviceContext);
    #pragma endregion

        grid.cull(camera, states, depthStencil, device, deviceContext, &resourceManager);

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

		ID3D11SamplerState * samplers[] = { skyRenderer.getSampler() };
		deviceContext->PSSetSamplers(1, 1, samplers);

		ID3D11Buffer *lightBuffs[] =
		{
			skyRenderer.getShaderBuffer(),
			skyRenderer.getLightMatrixBuffer()
		};
		
		deviceContext->PSSetConstantBuffers(1, 1, &lightBuffs[0]);
		deviceContext->VSSetConstantBuffers(2, 1, &lightBuffs[1]);

		ID3D11RenderTargetView * rtvs[] =
		{
			fakeBackBuffer,
			glowMap
		};
		deviceContext->OMSetRenderTargets(2, rtvs, depthStencil);
		
		draw();
		skyRenderer.renderSky(deviceContext, camera);

		ID3D11RenderTargetView * rtvNULL[2] = {nullptr};

        deviceContext->OMSetRenderTargets(2, rtvNULL, nullptr);

        ZeroMemory(SRVs, sizeof(SRVs));
        deviceContext->PSSetShaderResources(0, 4, SRVs);

        deviceContext->RSSetState(states->CullCounterClockwise());
        SHORT tabKeyState = GetAsyncKeyState(VK_TAB);
        if ((1 << 15) & tabKeyState)
        {
            this->drawToBackbuffer(grid.getDebugSRV());
        }
        drawGUI();
#endif

		///////Post effext
		postProcessor.addGlow(deviceContext, fakeBackBuffer, glowMap, &fakeBackBufferSwap);


		drawToBackbuffer(fakeBackBufferSwap);

        renderDebugInfo();
    }


    void Renderer::queueRender(RenderInfo * renderInfo)
    {
        if (renderQueue.size() > INSTANCE_CAP)
            throw "Renderer Exceeded Instance Cap.";

        renderQueue.push_back(renderInfo);
    }

    void Renderer::queueRenderDebug(RenderDebugInfo * debugInfo)
    {
        renderDebugQueue.push_back(debugInfo);
    }


    //void Graphics::Renderer::renderMenu(MenuInfo * info)
    //{
    //	this->spriteBatch->Begin();
    //	for (size_t i = 0; i < info->m_buttons.size(); i++)
    //	{
    //		/*this->spriteBatch->Draw(info->m_buttons.at(i)->m_texture, info->m_buttons.at(i)->m_rek);*/
    //	}
    //	this->spriteBatch->End();
 //  
    //}


    void Renderer::unloadMenuTextures()
    {
        if (menuTexturesLoaded == true)
        {
            SAFE_RELEASE(buttonTexture);
            SAFE_RELEASE(menuTexture);
            buttonTexture = nullptr;
            menuTexture = nullptr;
            menuTexturesLoaded = false;
        }
        
    }

    void Renderer::reloadMenuTextures()
    {
        if (menuTexturesLoaded == false)
        {
            ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, deviceContext, TEXTURE_PATH("menuTexture.png"), nullptr, &menuTexture));
            ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, deviceContext, TEXTURE_PATH("button.png"), nullptr, &buttonTexture));
            menuTexturesLoaded = true;
        }
    }

    //loads the textures for menu and GUI
    void Renderer::loadModellessTextures()
    {
       
        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, deviceContext, TEXTURE_PATH("menuTexture.png"), nullptr, &menuTexture));
        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, deviceContext, TEXTURE_PATH("button.png"), nullptr, &buttonTexture));
        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, deviceContext, TEXTURE_PATH("crosshair.png"), nullptr, &GUITexture1));
        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, deviceContext, TEXTURE_PATH("HPbar.png"), nullptr, &GUITexture2));

    }

    void Renderer::cull()
    {
        instanceQueue.clear();
        for (RenderInfo * info : renderQueue)
        {
            if (info->render)
            {
                instanceQueue[info->meshId].push_back({ info->translation });
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

    void Renderer::draw()
    {
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->VSSetConstantBuffers(3, 1, instanceOffsetBuffer);
        deviceContext->VSSetShaderResources(20, 1, instanceSBuffer);


        UINT instanceOffset = 0;
        for (InstanceQueue_t::value_type & pair : instanceQueue)
        {
            instanceOffsetBuffer.write(deviceContext, &instanceOffset, sizeof(UINT));
            instanceOffset += pair.second.size();

#if USE_TEMP_CUBE
            static TempCube tempCube(device);
			ModelInfo model = resourceManager.getModelInfo(CUBE);



			static UINT stride = sizeof(Vertex), offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, &tempCube.vertexBuffer, &stride, &offset);

			static ID3D11ShaderResourceView * modelTextures[3] = { nullptr };
			modelTextures[0] = model.diffuseMap;
			modelTextures[1] = model.normalMap;
			modelTextures[2] = model.specularMap;
			deviceContext->PSSetShaderResources(10, 3, modelTextures);

			deviceContext->DrawInstanced(36, (UINT)pair.second.size(), 0, 0);
#else
            ModelInfo model = resourceManager.getModelInfo(pair.first);

            static UINT stride = sizeof(Vertex), offset = 0;
            deviceContext->IASetVertexBuffers(0, 1, &model.vertexBuffer, &stride, &offset);
            deviceContext->IASetIndexBuffer(model.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

            static ID3D11ShaderResourceView * modelTextures[4] = { nullptr };
            modelTextures[0] = model.diffuseMap;
            modelTextures[1] = model.normalMap;
            modelTextures[2] = model.specularMap;
			modelTextures[3] = glowTest;
            deviceContext->PSSetShaderResources(10, 4, modelTextures);

            deviceContext->DrawIndexedInstanced((UINT)model.indexCount, (UINT)pair.second.size(), 0, 0, 0);
#endif
        }
    }

    void Renderer::drawToBackbuffer(ID3D11ShaderResourceView * texture)
    {
        float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        deviceContext->ClearRenderTargetView(backBuffer, clearColor);

        deviceContext->PSSetShaderResources(0, 1, &texture);

        UINT zero = 0;
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);

        deviceContext->IASetInputLayout(fullscreenQuad);
        deviceContext->VSSetShader(fullscreenQuad, nullptr, 0);
        deviceContext->PSSetShader(fullscreenQuad, nullptr, 0);

        static ID3D11SamplerState * pointClamp = states->PointClamp();
        deviceContext->PSSetSamplers(0, 1, &pointClamp);

        deviceContext->Draw(4, 0);

        ID3D11ShaderResourceView * srvNull = nullptr;
        deviceContext->PSSetShaderResources(0, 1, &srvNull);
    }

    void Renderer::drawGUI()
    {
     
        UINT stride = 20, offset = 0;
        deviceContext->IASetVertexBuffers(0, 1, &GUIvb, &stride, &offset);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->IASetInputLayout(GUIShader);

        float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        UINT sampleMask = 0xffffffff;
        deviceContext->OMSetBlendState(transparencyBlendState, blendFactor, sampleMask);
        deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);

        deviceContext->VSSetShader(GUIShader, nullptr, 0);

        deviceContext->PSSetShaderResources(0, 1, &GUITexture1);
        deviceContext->PSSetShaderResources(1, 1, &GUITexture2);
        deviceContext->PSSetShader(GUIShader, nullptr, 0);
       



        deviceContext->Draw(12, 0);


        ID3D11ShaderResourceView * SRVNULL = nullptr;
        deviceContext->PSSetShaderResources(0, 1, &SRVNULL);

    }

	
    //fills the button vertex buffer with button info;
    void Renderer::mapButtons(ButtonInfo * info)
    {
        //moves the buttons to ndc space
        TriangleVertex triangleVertices[6] =
        {
            2 *((float)((info->m_rek.x + info->m_rek.width )) / WIN_WIDTH) - 1, 2 * ((float)(WIN_HEIGHT - info->m_rek.y - info->m_rek.height) / WIN_HEIGHT) - 1, 0.0f,	//v0 pos
            1.0f, 1.0f,

            2 * ((float)(info->m_rek.x) / WIN_WIDTH) -1 , 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height)) / WIN_HEIGHT) - 1, 0.0f,	//v1
            0.0f, 1.0f,

            2 * ((float)(info->m_rek.x) / WIN_WIDTH) - 1 , 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height + info->m_rek.height)) / WIN_HEIGHT) - 1, 0.0f, //v2
            0.0f,  0.0f,

            //t2
            2 * ((float)(info->m_rek.x) / WIN_WIDTH) - 1 , 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height + info->m_rek.height)) / WIN_HEIGHT) - 1, 0.0f,	//v2 pos
            0.0f, 0.0f,

            2 * ((float)((info->m_rek.x + info->m_rek.width)) / WIN_WIDTH) - 1, 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height + info->m_rek.height)) / WIN_HEIGHT) - 1 , 0.0f,	//v3
            1.0f, 0.0f,

            2 * ((float)((info->m_rek.x + info->m_rek.width)) / WIN_WIDTH) -1, 2 * ((float)((WIN_HEIGHT - info->m_rek.y - info->m_rek.height)) / WIN_HEIGHT) -1 , 0.0f, //v0
            1.0f, 1.0f,
        };

        D3D11_MAPPED_SUBRESOURCE data = { 0 };
        ThrowIfFailed(deviceContext->Map(buttonQuad, 0, D3D11_MAP_WRITE_DISCARD, 0, &data));
        memcpy(data.pData, triangleVertices, sizeof(TriangleVertex) * 6);
        deviceContext->Unmap(buttonQuad, 0);

    }

    //draws the menu
    void Renderer::drawMenu(Graphics::MenuInfo * info)
    {
        reloadMenuTextures();

        //draws menu background
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        deviceContext->ClearRenderTargetView(backBuffer, clearColor);
        UINT stride = sizeof(Graphics::TriangleVertex) , offset = 0;
        deviceContext->RSSetViewports(1, &viewPort);
        deviceContext->IASetVertexBuffers(0, 1, &menuQuad, &stride, &offset);
        deviceContext->IASetInputLayout(menuShader);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->VSSetShader(menuShader, nullptr, 0);
        deviceContext->PSSetShader(menuShader, nullptr, 0);
        auto sampler = states->PointWrap();
        deviceContext->PSSetSamplers(0, 1, &sampler);
        deviceContext->PSSetShaderResources(0, 1, &menuTexture);

        deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);

        deviceContext->Draw(6, 0);

        //draws buttons
        deviceContext->PSSetShaderResources(0, 1, &buttonTexture);
        for (size_t i = 0; i < info->m_buttons.size(); i++)
        {
            mapButtons(&info->m_buttons.at(i));
            deviceContext->IASetVertexBuffers(0, 1, &buttonQuad, &stride, &offset);
            deviceContext->Draw(6, 0);
        }

        ID3D11ShaderResourceView * SRVNULL = nullptr;
        deviceContext->PSSetShaderResources(0, 1, &SRVNULL);

    }

    //creates a vetrex buffer for the GUI
    void Renderer::createGUIBuffers()
    {
        struct GUI
        {
            DirectX::SimpleMath::Vector2 verts;
            DirectX::SimpleMath::Vector2 uv;
            UINT element;
        };

        GUI GUIquad[12];
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


        GUIquad[6].verts = DirectX::SimpleMath::Vector2{ -1.0f, -1.0f };
        GUIquad[6].uv = DirectX::SimpleMath::Vector2{ 0.0f, 1.0f };

        GUIquad[7].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.8f };
        GUIquad[7].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

        GUIquad[8].verts = DirectX::SimpleMath::Vector2{ -0.8f, -1.0f };
        GUIquad[8].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

        GUIquad[9].verts = DirectX::SimpleMath::Vector2{ -0.8f, -0.8f };
        GUIquad[9].uv = DirectX::SimpleMath::Vector2{ 1.0f, 0.0f };

        GUIquad[10].verts = GUIquad[8].verts;
        GUIquad[10].uv = DirectX::SimpleMath::Vector2{ 1.0f, 1.0f };

        GUIquad[11].verts = GUIquad[7].verts;
        GUIquad[11].uv = DirectX::SimpleMath::Vector2{ 0.0f, 0.0f };

        GUIquad[6].element = 1;
        GUIquad[7].element = 1;
        GUIquad[8].element = 1;
        GUIquad[9].element = 1;
        GUIquad[10].element = 1;
        GUIquad[11].element = 1;

        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.ByteWidth = sizeof(GUIquad);

        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
        data.pSysMem = GUIquad;


        ThrowIfFailed(device->CreateBuffer(&desc, &data, &GUIvb));
    }

	

    //creates the vertexbuffers the menu uses.
    void Renderer::createMenuVBS()
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


        D3D11_BUFFER_DESC desc = {0};

        desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
        desc.ByteWidth = sizeof(TriangleVertex) * 6;

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = triangleVertices;

        ThrowIfFailed(device->CreateBuffer(&desc, &data, &menuQuad));

        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.Usage = D3D11_USAGE_DYNAMIC;

        ThrowIfFailed(device->CreateBuffer(&desc, &data, &buttonQuad));


    }

    void Renderer::renderDebugInfo()
    {
        if (renderDebugQueue.size() == 0) return;

        deviceContext->OMSetRenderTargets(1, &backBuffer, depthStencil);

        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
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

            deviceContext->OMSetDepthStencilState(info->useDepth ? states->DepthDefault() : states->DepthNone(), 0);
            deviceContext->Draw(info->points->size(), 0);
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
        BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
        BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;

        ThrowIfFailed(this->device->CreateBlendState(&BlendState, &transparencyBlendState));
    }


}
