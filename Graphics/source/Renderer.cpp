#include "Renderer.h"
#include <stdio.h>
#include "ThrowIfFailed.h"

using namespace Graphics;

Renderer::Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera)
    : device(device)
    , deviceContext(deviceContext)
    , backBuffer(backBuffer)
{
	TestQuad defferedTest[] =
	{
		DirectX::SimpleMath::Vector3(-1, -1, 0),
		DirectX::SimpleMath::Vector2(0, 1),
		DirectX::SimpleMath::Vector3(0, 0, -1),
		1,

		DirectX::SimpleMath::Vector3(-1, 1, 0),
		DirectX::SimpleMath::Vector2(0, 0),
		DirectX::SimpleMath::Vector3(0, 0, -1),
		1,

		DirectX::SimpleMath::Vector3(1, -1, 0),
		DirectX::SimpleMath::Vector2(1, 1),
		DirectX::SimpleMath::Vector3(0, 0, -1),
		1,

		DirectX::SimpleMath::Vector3(1, 1, 0),
		DirectX::SimpleMath::Vector2(1, 0),
		DirectX::SimpleMath::Vector3(0, 0, -1),
		1
	};

	createLightGrid(camera);

    createGBuffer();

	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}	
	};

	shaders[0] = shaderHandler.createVertexShader(device, L"FullscreenQuad.hlsl", "VS", desc, ARRAYSIZE(desc));
	shaders[1] = shaderHandler.createPixelhader(device, L"FullscreenQuad.hlsl", "PS");

	D3D11_INPUT_ELEMENT_DESC descDeffered[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "MATERIAL", 0, DXGI_FORMAT_R32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	defferedShaders[0] = shaderHandler.createVertexShader(device, L"Deffered.hlsl", "VS", descDeffered, ARRAYSIZE(descDeffered));
	defferedShaders[1] = shaderHandler.createPixelhader(device, L"Deffered.hlsl", "PS");

	D3D11_VIEWPORT viewPort;
	viewPort.Height = 720;
	viewPort.Width = 1280;
	viewPort.MaxDepth = 1.f;
	viewPort.MinDepth = 0.f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	deviceContext->RSSetViewports(1, &viewPort);

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = sizeof(FSQuadVerts);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = FSQuadVerts;

	ThrowIfFailed(device->CreateBuffer(&bufferDesc, &data, &FSQuad2));
	ThrowIfFailed( DirectX::CreateWICTextureFromFile(device, L"cat.jpg", nullptr, &view));


	bufferDesc.ByteWidth = sizeof(defferedTest);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	data.pSysMem = defferedTest;

	ThrowIfFailed(device->CreateBuffer(&bufferDesc, &data, &defferedTestBuffer));

	states = new DirectX::CommonStates(device);
}

Graphics::Renderer::~Renderer()
{
	view->Release();
	FSQuad2->Release();

	if (instanceBuffer)
		instanceBuffer->Release();

	if (gbuffer.depth)
		gbuffer.depth->Release();
	if (gbuffer.depthView)
		gbuffer.depthView->Release();
	gbuffer.diffuseSpec->Release();
	gbuffer.diffuseSpecView->Release();
	gbuffer.normalMat->Release();
	gbuffer.normalMatView->Release();
	gbuffer.position->Release();
	gbuffer.positionView->Release();
	defferedTestBuffer->Release();
	
}

void Graphics::Renderer::createLightGrid(Camera *camera)
{
	// TODO: organize better
#pragma region Temporary geometry setup
	DirectX::SimpleMath::Vector3 plane[] = {
		{ -10, 0, -10 },
		{ -10, 0,  10 },
		{  10, 0,  10 },

		{ -10, 0, -10 },
		{  10, 0,  10 },
		{  10, 0, -10 },
	};

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(plane);

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = plane;

		ThrowIfFailed(device->CreateBuffer(&desc, &data, &planeBuffer));
	}

	{
		D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		planeVS = shaderHandler.createVertexShader(device, L"Forward.hlsl", "VS", desc, 1);
		planePS = shaderHandler.createPixelhader(device, L"Forward.hlsl", "PS");
	}

	{
		ID3D11Texture2D* texture;
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = 1280;
		desc.Height = 720;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &texture));

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
		ZeroMemory(&descSRV, sizeof(descSRV));
		descSRV.Format = DXGI_FORMAT_R32_FLOAT;
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MipLevels = 1;

		ThrowIfFailed(device->CreateDepthStencilView(texture, &descDSV, &depthDSV));
		ThrowIfFailed(device->CreateShaderResourceView(texture, &descSRV, &depthSRV));
	}

#pragma endregion

#pragma region Grid params buffer
	gridParams.numThreadGroups[0] = ceil(1280 / (float)BLOCK_SIZE);
	gridParams.numThreadGroups[1] = ceil(720 / (float)BLOCK_SIZE);
	gridParams.numThreadGroups[2] = 1;

	gridParams.numThreads[0] = gridParams.numThreadGroups[0] * BLOCK_SIZE;
	gridParams.numThreads[1] = gridParams.numThreadGroups[1] * BLOCK_SIZE;
	gridParams.numThreads[2] = 1;

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(DispatchParams);

		D3D11_SUBRESOURCE_DATA data = { 0 };
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = &gridParams;

		ThrowIfFailed(device->CreateBuffer(&desc, &data, &gridParamsBuffer));
	}
#pragma endregion

#pragma region Generate grid frustums
	gridFrustumGenerationCS = shaderHandler.createComputeShader(device, L"LightGridGeneration.hlsl", "CS");

	{
		auto sz = sizeof(Frustum);
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.ByteWidth = sz * 3600;
		desc.StructureByteStride = sz;

		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, &gridFrustrums));
	}

	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = 0;
		desc.Buffer.Flags = 0;
		desc.Buffer.NumElements = 3600;

		ThrowIfFailed(device->CreateUnorderedAccessView(gridFrustrums, &desc, &gridFrustrumsUAV));
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		desc.BufferEx.NumElements = 3600;

		ThrowIfFailed(device->CreateShaderResourceView(gridFrustrums, &desc, &gridFrustrumsSRV));
	}



	shaderHandler.setComputeShader(gridFrustumGenerationCS, deviceContext);
	auto camera_buffer = camera->getBuffer();
	deviceContext->CSSetConstantBuffers(0, 1, &camera_buffer);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &gridFrustrumsUAV, 0);
	deviceContext->Dispatch(5, 3, 1);
	ID3D11UnorderedAccessView *reset = nullptr;
	deviceContext->CSSetUnorderedAccessViews(0, 1, &reset, 0);

#pragma endregion

#pragma region Create Lights
	{
		auto sz = sizeof(Light);
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.ByteWidth = sz * NUM_LIGHTS;
		desc.StructureByteStride = sz;

		Light lights[NUM_LIGHTS];
		ZeroMemory(lights, sizeof(lights));

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = lights;

		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, &gridLights));
	}

	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = 0;
		desc.Buffer.Flags = 0;
		desc.Buffer.NumElements = NUM_LIGHTS;

		ThrowIfFailed(device->CreateUnorderedAccessView(gridLights, &desc, &gridLightsUAV));
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		desc.BufferEx.NumElements = NUM_LIGHTS;

		ThrowIfFailed(device->CreateShaderResourceView(gridLights, &desc, &gridLightsSRV));
	}
#pragma endregion

	gridCullingCS = shaderHandler.createComputeShader(device, L"LightGridCulling.hlsl", "CS");

	#pragma region Grid Index Counter UAVs
	{
		uint32_t initialValue = 0;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.ByteWidth = sizeof(uint32_t);
		desc.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = &initialValue;

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.NumElements = 1;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Buffer.NumElements = 1;

		ID3D11Buffer *buffer;

		ThrowIfFailed(device->CreateBuffer(&desc, &data, &buffer));
		ThrowIfFailed(device->CreateUnorderedAccessView(buffer, &uavDesc, &gridResetIndexCounterUAV));
		ThrowIfFailed(device->CreateShaderResourceView(buffer, &srvDesc, &gridResetIndexCounterSRV));

		ThrowIfFailed(device->CreateBuffer(&desc, &data, &buffer));
		ThrowIfFailed(device->CreateUnorderedAccessView(buffer, &uavDesc, &gridOpaqueIndexCounterUAV));
		ThrowIfFailed(device->CreateShaderResourceView(buffer, &srvDesc, &gridOpaqueIndexCounterSRV));

		ThrowIfFailed(device->CreateBuffer(&desc, &data, &buffer));
		ThrowIfFailed(device->CreateUnorderedAccessView(buffer, &uavDesc, &gridTransparentIndexCounterUAV));
		ThrowIfFailed(device->CreateShaderResourceView(buffer, &srvDesc, &gridTransparentIndexCounterSRV));

	}
	#pragma endregion

	#pragma region Grid Index List UAVs
	{
		auto count = gridParams.numThreadGroups[0] * gridParams.numThreadGroups[1] * AVG_TILE_LIGHTS;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.ByteWidth = sizeof(uint32_t) * count;
		desc.StructureByteStride = sizeof(uint32_t);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.NumElements = count;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Buffer.NumElements = count;


		ID3D11Buffer *buffer;

		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, &buffer));
		ThrowIfFailed(device->CreateUnorderedAccessView(buffer, &uavDesc, &gridOpaqueIndexListUAV));
		ThrowIfFailed(device->CreateShaderResourceView(buffer, &srvDesc, &gridOpaqueIndexListSRV));

		ThrowIfFailed(device->CreateBuffer(&desc, nullptr, &buffer));
		ThrowIfFailed(device->CreateUnorderedAccessView(buffer, &uavDesc, &gridTransparentIndexListUAV));
		ThrowIfFailed(device->CreateShaderResourceView(buffer, &srvDesc, &gridTransparentIndexListSRV));

	}
	#pragma endregion

	#pragma region Light Grid UAVs
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32_UINT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.Width = gridParams.numThreadGroups[0];
		desc.Height = gridParams.numThreadGroups[1];
		desc.SampleDesc.Count = 1;
		desc.ArraySize = 1;

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.Format = DXGI_FORMAT_R32G32_UINT;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32_UINT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;


		ID3D11Texture2D *texture;

		ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &texture));
		ThrowIfFailed(device->CreateUnorderedAccessView(texture, &uavDesc, &gridOpaqueLightGridUAV));
		ThrowIfFailed(device->CreateShaderResourceView(texture, &srvDesc, &gridOpaqueLightGridSRV));

		ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &texture));
		ThrowIfFailed(device->CreateUnorderedAccessView(texture, &uavDesc, &gridTransparentLightGridUAV));
		ThrowIfFailed(device->CreateShaderResourceView(texture, &srvDesc, &gridTransparentLightGridSRV));

	}

	#pragma endregion

	#pragma region Debug heatmap

	ID3D11Texture2D *texture;

	ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, L"heatmap.png", nullptr, &gradientSRV));

	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// TODO: hardcoded
		desc.Width = 1280;
		desc.Height = 720;
		desc.SampleDesc.Count = 1;
		desc.ArraySize = 1;

		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;

		ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &texture));
	}
		
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		ThrowIfFailed(device->CreateUnorderedAccessView(texture, &desc, &gridDebugUAV));
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		ThrowIfFailed(device->CreateShaderResourceView(texture, &desc, &gridDebugSRV));
	}
	#pragma endregion
}

void Graphics::Renderer::cullLightGrid(Camera * camera)
{
	shaderHandler.setComputeShader(gridCullingCS, deviceContext);

	ID3D11Buffer *bufs[] = {
		camera->getBuffer(),
		gridParamsBuffer
	};
	deviceContext->CSSetConstantBuffers(0, 2, bufs);

	ID3D11ShaderResourceView *SRVs[] = {
		depthSRV,
		gridFrustrumsSRV,
		gridLightsSRV,
		gradientSRV
	};
	deviceContext->CSSetShaderResources(0, 4, SRVs);
	auto sampler = states->LinearClamp();
	deviceContext->CSSetSamplers(0, 1, &sampler);
	
	ID3D11UnorderedAccessView *UAVs[] = {
		gridOpaqueIndexCounterUAV,
		gridTransparentIndexCounterUAV,
		gridOpaqueIndexListUAV,
		gridTransparentIndexListUAV,
		gridOpaqueLightGridUAV,
		gridTransparentLightGridUAV,
		gridDebugUAV
	};
	deviceContext->CSSetUnorderedAccessViews(0, 7, UAVs, nullptr);
	
	deviceContext->Dispatch(
		gridParams.numThreadGroups[0],
		gridParams.numThreadGroups[1],
		1
	);

	ZeroMemory(UAVs, sizeof(UAVs));
	ZeroMemory(SRVs, sizeof(SRVs));
	deviceContext->CSSetShaderResources(0, 4, SRVs);
	deviceContext->CSSetUnorderedAccessViews(0, 7, UAVs, nullptr);
	deviceContext->CSSetShader(nullptr, nullptr, 0);
}

float f;

void Graphics::Renderer::drawForward(Camera *camera)
{
	f += 0.001;
	camera->update({ sin(f) * 30, 10, cos(f) * 10 }, { 0, 0, 0 }, deviceContext);

	shaderHandler.setShaders(planeVS, -1, -1, deviceContext);

	UINT stride = sizeof(float) * 3;
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &planeBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	auto buf = camera->getBuffer();
	deviceContext->VSSetConstantBuffers(0, 1, &buf);
	deviceContext->OMSetRenderTargets(0, nullptr, depthDSV);
	deviceContext->Draw(6, 0);

	deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	cullLightGrid(camera);
}

void Renderer::render(Camera * camera)
{
    /*
    //setCamera(sun);
    //setShader(shadow);
    //setRenderTarget(shadowMap);
    //for (RenderInfo info : renderQueue)
    //{
    //  draw(info);
    //}

    setCamera(camera);
    setShader(deffered);
    
*/

    //deviceContext->PSSetConstantBuffers(0, 3, nullptr);
    //deviceContext->OMSetRenderTargets(3, (ID3D11RenderTargetView * const *)&gbuffer, gbuffer.depth);
    

    ID3D11Buffer *cameraBuffer[] = { camera->getBuffer() };
    deviceContext->VSSetConstantBuffers(0, 1, cameraBuffer);    
    cull();
    //draw();

	drawForward(camera);

	//temp
	this->drawDeffered();
	this->drawToBackbuffer(gridDebugSRV);
}

void Renderer::qeueuRender(RenderInfo * renderInfo)
{
    renderQueue.push_back(renderInfo);
}

void Renderer::createGBuffer()
{
    D3D11_TEXTURE2D_DESC textureDesc = { 0 };
    textureDesc.Width = 1280;
    textureDesc.Height = 720;
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

	diffuseSpecTexture->Release();
	normalMatTexture->Release();
	positionTexture->Release();
}

void Renderer::cull()
{
    for (RenderInfo * info : renderQueue)
    {
        if (info->render)
        {
            instanceQueue[info->meshId].push_back({ info->translation });
        }
    }
    renderQueue.clear();
}

void Renderer::draw()
{
    // Sort instance id from all meshes
    D3D11_MAPPED_SUBRESOURCE data = { 0 };

	//TODO: DEN DÄR SAKEN BEHÖVEER NOG INITIERAS FÖRST (instanceBuffer)
    deviceContext->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

    DWORD offset = 0;
    for (InstanceQueue_t::value_type & pair : instanceQueue)
    {
        memcpy((void*)((DWORD)data.pData + offset), &pair.second.begin(), sizeof(pair.second));
        offset += sizeof(pair.second);
    }
    
    deviceContext->Unmap(instanceBuffer, 0);


    // draw all instanced meshes
    for (InstanceQueue_t::value_type & pair : instanceQueue)
    {
        //getVertexBuffer(pair.first);
        //deviceContext->DrawInstanced()
    }
}

void Renderer::drawDeffered()
{
	
	ID3D11RenderTargetView * RTVS[] = 
	{
		gbuffer.diffuseSpec,
		gbuffer.normalMat,
		gbuffer.position
	};

	deviceContext->OMSetRenderTargets(3, RTVS, nullptr);
	shaderHandler.setShaders(defferedShaders[0], NO_SHADER, defferedShaders[1], deviceContext);

	//textur här typ

	UINT stride = 36, offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &defferedTestBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->Draw(4, 0);

	ID3D11RenderTargetView * RTVNULLS[] =
	{
		NULL,
		NULL,
		NULL
	};
	deviceContext->OMSetRenderTargets(3, RTVNULLS, NULL);
}

void Graphics::Renderer::drawToBackbuffer(ID3D11ShaderResourceView * texture)
{
    deviceContext->PSSetShaderResources(0, 1, &texture);
    UINT stride = sizeof(DirectX::SimpleMath::Vector2), offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &FSQuad2, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->OMSetRenderTargets(1, &backBuffer, nullptr);
    
	shaderHandler.setShaders(shaders[0], NO_SHADER, shaders[1], deviceContext);

	deviceContext->Draw(4, 0);


	ID3D11ShaderResourceView * SRVNULL = nullptr;
	deviceContext->PSSetShaderResources(0, 1, &SRVNULL);
}
