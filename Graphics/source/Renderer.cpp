#include "Renderer.h"
#include <stdio.h>
#include "ThrowIfFailed.h"
#include <Engine\Constants.h>

using namespace Graphics;

Renderer::Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer, Camera *camera)
    : device(device)
    , deviceContext(deviceContext)
    , backBuffer(backBuffer)
	, resourceManager(device)
{
	using namespace DirectX::SimpleMath;

	TestCube defferedTest[] =
	{
		//FORWARD
		Vector3(-1, -1, -1),
		Vector2(0, 0),
		Vector3(0, 0, -1),
		1,

		Vector3(-1, 1, -1),
		Vector2(0, 1),
		Vector3(0, 0, -1),
		1,

		Vector3(1, -1, -1),
		Vector2(1, 0),
		Vector3(0, 0, -1),
		1,

		Vector3(1, -1, -1),
		Vector2(1, 0),
		Vector3(0, 0, -1),
		1,

		Vector3(-1, 1, -1),
		Vector2(0, 1),
		Vector3(0, 0, -1),
		1,
		
		Vector3(1, 1, -1),
		Vector2(1, 1),
		Vector3(0, 0, -1),
		1,
		

		////// TOP
		Vector3(-1, 1, -1),
		Vector2(0, 0),
		Vector3(0, 1, 0),
		1,

		Vector3(-1, 1, 1),
		Vector2(0, 1),
		Vector3(0, 1, 0),
		1,

		Vector3(1, 1, -1),
		Vector2(1, 0),
		Vector3(0, 1, 0),
		1,

		Vector3(1, 1, -1),
		Vector2(1, 0),
		Vector3(0, 1, 0),
		1,

		Vector3(-1, 1, 1),
		Vector2(0, 1),
		Vector3(0, 1, 0),
		1,

		Vector3(1, 1, 1),
		Vector2(1, 1),
		Vector3(0, 1, 0),
		1,
		/////// LEFT

		Vector3(-1, -1, 1),
		Vector2(0, 1),
		Vector3(-1, 0, 0),
		1,

		Vector3(-1, 1, 1),
		Vector2(1, 1),
		Vector3(-1, 0, 0),
		1,

		Vector3(-1, -1, -1),
		Vector2(0, 0),
		Vector3(-1, 0, 0),
		1,

		Vector3(-1, -1, -1),
		Vector2(0, 0),
		Vector3(-1, 0, 0),
		1,

		Vector3(-1, 1, 1),
		Vector2(1, 1),
		Vector3(-1, 0, 0),
		1,

		Vector3(-1, 1, -1),
		Vector2(1, 0),
		Vector3(-1, 0, 0),
		1,

		/////RIGHT

		Vector3(1, 1, 1),
		Vector2(1, 1),
		Vector3(1, 0, 0),
		1,

		Vector3(1, -1, 1),
		Vector2(0, 1),
		Vector3(1, 0, 0),
		1,

		Vector3(1, -1, -1),
		Vector2(0, 0),
		Vector3(1, 0, 0),
		1,

		Vector3(1, 1, 1),
		Vector2(1, 1),
		Vector3(1, 0, 0),
		1,
		
		Vector3(1, -1, -1),
		Vector2(0, 0),
		Vector3(1, 0, 0),
		1,

		Vector3(1, 1, -1),
		Vector2(1, 0),
		Vector3(1, 0, 0),
		1,
			//////BACK

		
		Vector3(-1, 1, 1),
		Vector2(0, 1),
		Vector3(0, 0, 1),
		1,

		Vector3(-1, -1, 1),
		Vector2(0, 0),
		Vector3(0, 0, 1),
		1,

		Vector3(1, -1, 1),
		Vector2(1, 0),
		Vector3(0, 0, 1),
		1,

		Vector3(-1, 1, 1),
		Vector2(0, 1),
		Vector3(0, 0, 1),
		1,

		Vector3(1, -1, 1),
		Vector2(1, 0),
		Vector3(0, 0, 1),
		1,

		Vector3(1, 1, 1),
		Vector2(1, 1),
		Vector3(0, 0, 1),
		1,

		//////BOTTOM
		Vector3(-1, -1, 1),
		Vector2(0, 1),
		Vector3(0, -1, 0),
		1,

		Vector3(-1, -1, -1),
		Vector2(0, 0),
		Vector3(0, -1, 0),
		1,

		Vector3(1, -1, -1),
		Vector2(1, 0),
		Vector3(0, -1, 0),
		1,

		Vector3(-1, -1, 1),
		Vector2(0, 1),
		Vector3(0, -1, 0),
		1,

		Vector3(1, -1, -1),
		Vector2(1, 0),
		Vector3(0, -1, 0),
		1,

		Vector3(1, -1, 1),
		Vector2(1, 1),
		Vector3(0, -1, 0),
		1
	};

    createGBuffer();
	createCubeInstances();
	createDepthStencil();

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
	ThrowIfFailed( DirectX::CreateWICTextureFromFile(device, TEXTURE_PATH("cat.jpg"), nullptr, &view));


	bufferDesc.ByteWidth = sizeof(defferedTest);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	data.pSysMem = defferedTest;

	ThrowIfFailed(device->CreateBuffer(&bufferDesc, &data, &defferedTestBuffer));

	states = new DirectX::CommonStates(device);
	grid.initialize(camera, device, deviceContext, &resourceManager);
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
	
	delete states;
}

float f = 59.42542;

void Graphics::Renderer::drawForward(Camera *camera)
{
	FLOAT col[4] = { 0, 0, 0, 1 };
	deviceContext->ClearRenderTargetView(backBuffer, col);
	deviceContext->ClearDepthStencilView(dSV, D3D11_CLEAR_DEPTH, 1.f, 0);

	//f += 0.01;
	//camera->update({ sin(f) * 30, 10, cos(f) * 10 }, { 0, 0, 0 }, deviceContext);

	resourceManager.setShaders(VERTEX_FORWARD_PLUS, (PixelShaderID)-1, deviceContext);


	UINT stride = 36, offset = 0;
	UINT instanceStride = 12;
	deviceContext->IASetVertexBuffers(0, 1, &defferedTestBuffer, &stride, &offset);
	deviceContext->IASetVertexBuffers(1, 1, &instanceBuffer, &instanceStride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);
	deviceContext->OMSetRenderTargets(0, nullptr, dSV);

	deviceContext->DrawInstanced(36, 3, 0, 0);
	deviceContext->OMSetRenderTargets(0, nullptr, nullptr);


	auto lights = grid.getLights();

	Light *ptr = lights->map(deviceContext);

	for (int i = 0; i < NUM_LIGHTS; i++) {
		ptr[i].color = DirectX::SimpleMath::Vector3(
			((unsigned char)(5+i * 53 * i + 4)) / 255.f,
			((unsigned char)(66 + i * 23 + 4)) / 255.f,
			((unsigned char)(11 + i * 455 + 4)) / 255.f
		);
		ptr[i].positionWS = DirectX::SimpleMath::Vector3(0.f, 1.f, 1.f - 1/8.f - i / 4.f);//ptr[i].color * 2 * 3;
		//ptr[i].positionWS.x = sin(i+f*0.01) * ptr[i].positionWS.x*2;
		//ptr[i].positionWS.y = 0.5f;
		//ptr[i].positionWS.z = cos(i+f*0.01) * ptr[i].positionWS.z*2;
		
		ptr[i].positionVS = DirectX::SimpleMath::Vector4::Transform(DirectX::SimpleMath::Vector4(ptr[i].positionWS.x, ptr[i].positionWS.y, ptr[i].positionWS.z, 1.f), camera->getView());
		ptr[i].range = 1/8.f;// 1.f + ((unsigned char)(i * 53 * i + 4)) / 255.f * i;
		ptr[i].intensity = 1.f;
	}

	lights->unmap(deviceContext);

	grid.cull(camera, states, depthSRV, device, deviceContext, &resourceManager);

	//cullLightGrid(camera);

	resourceManager.setShaders(VERTEX_FORWARD_PLUS, PIXEL_FORWARD_PLUS, deviceContext);

	ID3D11ShaderResourceView *SRVs[] = {
		grid.getOpaqueIndexList()->getSRV(),
		grid.getOpaqueLightGridSRV(),
		grid.getLights()->getSRV(),
		view
	};
	auto sampler = states->LinearClamp();
	deviceContext->PSSetShaderResources(0, 4, SRVs);
	deviceContext->PSSetSamplers(0, 1, &sampler);
	deviceContext->OMSetRenderTargets(1, &backBuffer, dSV);
	deviceContext->DrawInstanced(36, 3, 0, 0);

	ZeroMemory(SRVs, sizeof(SRVs));
	deviceContext->PSSetShaderResources(0, 3, SRVs);
}

void Renderer::render(Camera * camera)
{
	deviceContext->RSSetState(states->CullClockwise());

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
    
    ID3D11Buffer *cameraBuffer = camera->getBuffer();
    deviceContext->VSSetConstantBuffers(0, 1, &cameraBuffer);    
    cull();
    //draw();

	drawForward(camera);
	
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

void Graphics::Renderer::createDepthStencil()
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
	descSten.DepthFunc = D3D11_COMPARISON_LESS;
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

void Graphics::Renderer::createCubeInstances()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = 12 * 3;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	DirectX::SimpleMath::Vector3 offsets[] =
	{
		DirectX::SimpleMath::Vector3(0, 0, 0),
		DirectX::SimpleMath::Vector3(3, 4, 0),
		DirectX::SimpleMath::Vector3(-3, 0, 5)
	};

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = offsets;

	ThrowIfFailed(device->CreateBuffer(&desc, &data, &instanceBuffer));

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
		//resourceManager->getVertexBuffer(pair.first);
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
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	deviceContext->ClearRenderTargetView(gbuffer.diffuseSpec, clearColor);
	deviceContext->ClearRenderTargetView(gbuffer.normalMat, clearColor);
	deviceContext->ClearRenderTargetView(gbuffer.position, clearColor);
	deviceContext->ClearDepthStencilView(dSV, D3D11_CLEAR_DEPTH, 1, 0);

	deviceContext->OMSetRenderTargets(3, RTVS, dSV);
	deviceContext->PSSetShaderResources(0, 1, &this->view);

	resourceManager.setShaders(VertexShaderID::VERTEX_DEFFERED, PixelShaderID::PIXEL_DEFFERED, deviceContext);
	resourceManager.setSampler(SamplerID::pointSampler, deviceContext);
	//textur här typ

	UINT stride = 36, offset = 0;
	UINT instanceStride = 12;
	deviceContext->IASetVertexBuffers(0, 1, &defferedTestBuffer, &stride, &offset);
	deviceContext->IASetVertexBuffers(1, 1, &instanceBuffer, &instanceStride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->DrawInstanced(36, 3, 0, 0);

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
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//deviceContext->ClearRenderTargetView(backBuffer, clearColor);

	ID3D11ShaderResourceView * SRVS[] =
	{
		texture,
		gbuffer.normalMatView,
		gbuffer.positionView
	};

	deviceContext->PSSetShaderResources(0, 3, SRVS);

    UINT stride = sizeof(DirectX::SimpleMath::Vector2), offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &FSQuad2, &stride, &offset);
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
