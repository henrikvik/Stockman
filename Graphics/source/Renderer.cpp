#include "Renderer.h"
#include <stdio.h>
#include "ThrowIfFailed.h"
#include <Engine\Constants.h>


namespace Graphics
{


	Renderer::Renderer(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera)
		: simpleForward(gDevice, SHADER_PATH("SimpleForward.hlsl"), VERTEX_INSTANCE_DESC)
		, cube(gDevice)
	{
		this->device = gDevice;
		this->deviceContext = gDeviceContext;
		this->backBuffer = backBuffer;

		createDepthStencil();
		createGBuffer();
		createInstanceBuffer();
		initialize(gDevice, gDeviceContext);

		viewPort = { 0 };
		viewPort.Width = WIN_WIDTH;
		viewPort.Height = WIN_HEIGHT;
		viewPort.MaxDepth = 1.f;

		states = new DirectX::CommonStates(device);
		grid.initialize(camera, device, deviceContext, &resourceManager);

    }


    Renderer::~Renderer()
    {
        SAFE_RELEASE(instanceBuffer);
        gbuffer.Release();
		delete states;

    }

    void Renderer::initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext)
    {
        resourceManager.initialize(gDevice, gDeviceContext);
    }


	float f = 59.42542;

	void Renderer::render(Camera * camera)
	{
		cull();

		ID3D11Buffer *cameraBuffer = camera->getBuffer();
		deviceContext->VSSetConstantBuffers(0, 1, &cameraBuffer);

		static float clearColor[4] = { 0,0,0,1 };
		deviceContext->ClearRenderTargetView(backBuffer, clearColor);
		deviceContext->ClearDepthStencilView(dSV, D3D11_CLEAR_DEPTH, 1.f, 0);

		/*deviceContext->ClearDepthStencilView(gbuffer.depth, D3D11_CLEAR_DEPTH, 1, 0);
		deviceContext->OMSetRenderTargets(1, &backBuffer, gbuffer.depth);

		deviceContext->RSSetViewports(1, &viewPort);

		static ID3D11RasterizerState * rsState = [&]() {
			D3D11_RASTERIZER_DESC desc = {};
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_BACK;
			desc.FrontCounterClockwise = false;

			ID3D11RasterizerState * rsState;
			ThrowIfFailed(device->CreateRasterizerState(&desc, &rsState));

			return rsState;
		}();

		deviceContext->RSSetState(rsState);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		simpleForward.setShader(deviceContext);
		draw();


		drawForward(camera);*/

		deviceContext->RSSetViewports(1, &viewPort);
		deviceContext->RSSetState(states->CullClockwise());

		resourceManager.setShaders(VERTEX_FORWARD_PLUS, (PixelShaderID)-1, deviceContext);
		deviceContext->OMSetRenderTargets(0, nullptr, dSV);
		
		draw();

		deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

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
			ptr[i].positionWS.x = sin(f) * ptr[i].positionWS.x * 2;
			ptr[i].positionWS.y = 0.1f;
			ptr[i].positionWS.z = cos(f) * ptr[i].positionWS.z * 2;

			//ptr[i].positionWS = DirectX::SimpleMath::Vector3(0.f, 1.f, 1.f - 1 / 8.f - i / 4.f);
			ptr[i].positionVS = DirectX::SimpleMath::Vector4::Transform(DirectX::SimpleMath::Vector4(ptr[i].positionWS.x, ptr[i].positionWS.y, ptr[i].positionWS.z, 1.f), camera->getView());
			ptr[i].range = i / 4.f;// 1.f + ((unsigned char)(i * 53 * i + 4)) / 255.f * i;
			ptr[i].intensity = 1.f;
		}
		lights->unmap(deviceContext);

		grid.cull(camera, states, depthSRV, device, deviceContext, &resourceManager);

		resourceManager.setShaders(VERTEX_FORWARD_PLUS, PIXEL_FORWARD_PLUS, deviceContext);

		ID3D11ShaderResourceView *SRVs[] = {
			grid.getOpaqueIndexList()->getSRV(),
			grid.getOpaqueLightGridSRV(),
			grid.getLights()->getSRV(),
		};
		auto sampler = states->LinearClamp();
		deviceContext->PSSetShaderResources(0, 3, SRVs);
		deviceContext->PSSetSamplers(0, 1, &sampler);
		deviceContext->OMSetRenderTargets(1, &backBuffer, dSV);
		
		draw();

		deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		ZeroMemory(SRVs, sizeof(SRVs));
		deviceContext->PSSetShaderResources(0, 3, SRVs);

	
		//temp
		//this->drawDeffered();
		//this->drawToBackbuffer(gbuffer.positionView);
		deviceContext->RSSetState(states->CullCounterClockwise());
		SHORT tabKeyState = GetAsyncKeyState(VK_TAB);
		if ((1 << 15) & tabKeyState)
		{
			this->drawToBackbuffer(grid.getDebugSRV());
		}
	}

    void Renderer::queueRender(RenderInfo * renderInfo)
    {
        if (renderQueue.size() > INSTANCE_CAP)
            throw "Renderer Exceeded Instance Cap.";

        renderQueue.push_back(renderInfo);
    }



    void Renderer::createGBuffer()
    {
        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = WIN_WIDTH;
        textureDesc.Height = WIN_HEIGHT;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.ArraySize = 1;

        ID3D11Texture2D * diffuseSpecTexture;
        ID3D11Texture2D * normalMatTexture;
        ID3D11Texture2D * positionTexture;

        ThrowIfFailed(device->CreateTexture2D(&textureDesc, nullptr, &diffuseSpecTexture));
        ThrowIfFailed(device->CreateTexture2D(&textureDesc, nullptr, &normalMatTexture));
        ThrowIfFailed(device->CreateTexture2D(&textureDesc, nullptr, &positionTexture));
        ThrowIfFailed(device->CreateRenderTargetView(diffuseSpecTexture, nullptr, &gbuffer.diffuseSpec));
        ThrowIfFailed(device->CreateRenderTargetView(normalMatTexture, nullptr, &gbuffer.normalMat));
        ThrowIfFailed(device->CreateRenderTargetView(positionTexture, nullptr, &gbuffer.position));
        ThrowIfFailed(device->CreateShaderResourceView(diffuseSpecTexture, nullptr, &gbuffer.diffuseSpecView));
        ThrowIfFailed(device->CreateShaderResourceView(normalMatTexture, nullptr, &gbuffer.normalMatView));
        ThrowIfFailed(device->CreateShaderResourceView(positionTexture, nullptr, &gbuffer.positionView));


        D3D11_TEXTURE2D_DESC depthTexDesc = {};
        depthTexDesc.Width = WIN_WIDTH;
        depthTexDesc.Height = WIN_HEIGHT;
        depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
        depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        depthTexDesc.SampleDesc.Count = 1;
        depthTexDesc.ArraySize = 1;

        D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc = {};
        depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthDesc.Texture2D.MipSlice = 0;
        depthDesc.Flags = 0;

        ID3D11Texture2D * depthTexture;
        ThrowIfFailed(device->CreateTexture2D(&depthTexDesc, nullptr, &depthTexture));
        ThrowIfFailed(device->CreateDepthStencilView(depthTexture, &depthDesc, &gbuffer.depth));

        diffuseSpecTexture->Release();
        normalMatTexture->Release();
        positionTexture->Release();
        depthTexture->Release();
    }

    void Renderer::createInstanceBuffer()
    {
        D3D11_BUFFER_DESC instanceDesc = { 0 };
        instanceDesc.ByteWidth = sizeof(InstanceData) * INSTANCE_CAP;
        instanceDesc.Usage = D3D11_USAGE_DYNAMIC;
        instanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        instanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        ThrowIfFailed(device->CreateBuffer(&instanceDesc, nullptr, &instanceBuffer));
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

	// TEMP
	void Renderer::createDepthStencil()
	{
		D3D11_TEXTURE2D_DESC descTex;
		ZeroMemory(&descTex, sizeof(descTex));
		descTex.ArraySize = descTex.MipLevels = 1;
		descTex.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		descTex.Format = DXGI_FORMAT_R32_TYPELESS;
		descTex.Height = 720;
		descTex.Width = 1280;
		descTex.SampleDesc.Count = 1;
		descTex.MipLevels = 1;

		ID3D11Texture2D* texture;

		ThrowIfFailed(this->device->CreateTexture2D(&descTex, NULL, &texture));


		D3D11_DEPTH_STENCIL_DESC descSten;
		ZeroMemory(&descSten, sizeof(D3D11_DEPTH_STENCIL_DESC));
		descSten.DepthEnable = true;
		descSten.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		descSten.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descSten.StencilEnable = false;

		ThrowIfFailed(this->device->CreateDepthStencilState(&descSten, &this->dSS));


		D3D11_DEPTH_STENCIL_VIEW_DESC descStenV;
		ZeroMemory(&descStenV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		descStenV.Format = DXGI_FORMAT_D32_FLOAT;
		descStenV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		ThrowIfFailed(this->device->CreateDepthStencilView(texture, &descStenV, &this->dSV));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		ThrowIfFailed(this->device->CreateShaderResourceView(texture, &srvDesc, &this->depthSRV));

		this->deviceContext->OMSetDepthStencilState(this->dSS, 0);

		texture->Release();
	}

	void Renderer::draw()
	{
		// Sort instance id from all meshes
		D3D11_MAPPED_SUBRESOURCE instanceMap = { 0 };

        deviceContext->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &instanceMap);

        char* dest = (char*)instanceMap.pData;
        for (InstanceQueue_t::value_type & pair : instanceQueue)
        {
            void * data = pair.second.data();
            size_t size = pair.second.size() * sizeof(InstanceData);
            memcpy(dest, data, size);
            dest += size;
        }

        deviceContext->Unmap(instanceBuffer, 0);

        // draw all instanced meshes
        UINT readOffset = 0;
        UINT offsets[2] = { 0, 0 };
        UINT strides[2] = { sizeof(Vertex), sizeof(InstanceData) };
        ID3D11Buffer * buffers[2] = { nullptr, instanceBuffer };

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        for (InstanceQueue_t::value_type & pair : instanceQueue)
        {
#if true
            ModelInfo model = resourceManager.getModelInfo(pair.first);

            buffers[0] = model.vertexBuffer;
            deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
            deviceContext->IASetIndexBuffer(model.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

            deviceContext->DrawIndexedInstanced((UINT)model.indexCount, (UINT)pair.second.size(), 0, 0, readOffset);
            readOffset += (UINT)pair.second.size() * sizeof(InstanceData);
#else
            buffers[0] = cube.vertexBuffer;
            deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
            deviceContext->DrawInstanced(36, (UINT)pair.second.size(), 0, 0);
#endif

        }

    }

    void Renderer::drawToBackbuffer(ID3D11ShaderResourceView * texture)
    {
        float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        deviceContext->ClearRenderTargetView(backBuffer, clearColor);

        ID3D11ShaderResourceView * SRVS[] =
        {
            texture,
            gbuffer.normalMatView,
            gbuffer.positionView
        };

        deviceContext->PSSetShaderResources(0, 3, SRVS);

        UINT zero = 0;
        //deviceContext->IASetVertexBuffers(0, 1, nullptr, &zero, &zero);
        //deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, zero);
        //deviceContext->IASetInputLayout(nullptr);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);

        resourceManager.setShaders(VertexShaderID::VERTEX_QUAD, PixelShaderID::PIXEL_QUAD, deviceContext);
        resourceManager.setSampler(SamplerID::pointSampler, deviceContext);

        deviceContext->Draw(4, 0);

        ID3D11ShaderResourceView * SRVNULLS[] =
        {
            NULL,
            NULL,
            NULL
        };
        deviceContext->PSSetShaderResources(0, 3, SRVNULLS);
    }

    void Renderer::drawGUI()
    {
        /*deviceContext->PSSetShaderResources(0, 1, &GUI);
        deviceContext->PSSetShaderResources(1, 1, &view);*/
        UINT stride = 12, offset = 0;
        deviceContext->IASetVertexBuffers(0, 1, &GUIvb, &stride, &offset);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        UINT sampleMask = 0xffffffff;
        deviceContext->OMSetBlendState(transparencyBlendState, blendFactor, sampleMask);
        deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);


        //resourceManager.setShaders(VertexShaderID::VERTEX_GUI, PixelShaderID::PIXEL_GUI, deviceContext);

        deviceContext->Draw(12, 0);


        ID3D11ShaderResourceView * SRVNULL = nullptr;
        deviceContext->PSSetShaderResources(0, 1, &SRVNULL);

    }

    void Renderer::createGUIBuffers()
    {
        struct GUI
        {
            DirectX::SimpleMath::Vector2 verts;
            UINT element;
        };

        GUI GUIquad[12];
        GUIquad[0].verts = DirectX::SimpleMath::Vector2{ -0.1f, -0.1f };
        GUIquad[1].verts = DirectX::SimpleMath::Vector2{ -0.1f, 0.1f };
        GUIquad[2].verts = DirectX::SimpleMath::Vector2{ 0.1f, -0.1f };
        GUIquad[3].verts = DirectX::SimpleMath::Vector2{ 0.1f, 0.1f };
        GUIquad[4].verts = GUIquad[2].verts;
        GUIquad[5].verts = GUIquad[1].verts;

        GUIquad[0].element = 0;
        GUIquad[1].element = 0;
        GUIquad[2].element = 0;
        GUIquad[3].element = 0;
        GUIquad[4].element = 0;
        GUIquad[5].element = 0;


        GUIquad[6].verts = DirectX::SimpleMath::Vector2{ -1.0f, -1.0f };
        GUIquad[7].verts = DirectX::SimpleMath::Vector2{ -1.0f, -0.8f };
        GUIquad[8].verts = DirectX::SimpleMath::Vector2{ -0.7f, -1.0f };
        GUIquad[9].verts = DirectX::SimpleMath::Vector2{ -0.7f, -0.8f };
        GUIquad[10].verts = GUIquad[8].verts;
        GUIquad[11].verts = GUIquad[7].verts;

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
