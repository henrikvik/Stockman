#include "Renderer.h"
#include <stdio.h>
#include "ThrowIfFailed.h"
#include <Engine\Constants.h>


using namespace Graphics;

Renderer::Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11RenderTargetView * backBuffer)
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

	dSS->Release();
	dSV->Release();


	
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
    
    ID3D11Buffer *cameraBuffer = camera->getBuffer();
    deviceContext->VSSetConstantBuffers(0, 1, &cameraBuffer);    
    cull();
    //draw();
	
	//temp
	this->drawDeffered();
	this->drawToBackbuffer(gbuffer.positionView);
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
	descTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descTex.Format = DXGI_FORMAT_D32_FLOAT;
	descTex.Height = 720;
	descTex.Width = 1280;
	descTex.SampleDesc.Count = 1;

	ID3D11Texture2D* texture;

	this->device->CreateTexture2D(&descTex, NULL, &texture);


	D3D11_DEPTH_STENCIL_DESC descSten;
	ZeroMemory(&descSten, sizeof(D3D11_DEPTH_STENCIL_DESC));
	descSten.DepthEnable = true;
	descSten.DepthFunc = D3D11_COMPARISON_LESS;
	descSten.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descSten.StencilEnable = false;

	this->device->CreateDepthStencilState(&descSten, &this->dSS);


	D3D11_DEPTH_STENCIL_VIEW_DESC descStenV;
	ZeroMemory(&descStenV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descStenV.Format = DXGI_FORMAT_D32_FLOAT;
	descStenV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	this->device->CreateDepthStencilView(texture, &descStenV, &this->dSV);


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

void Graphics::Renderer::drawGUI()
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


	resourceManager.setShaders(VertexShaderID::VERTEX_GUI, PixelShaderID::PIXEL_GUI, deviceContext);

	deviceContext->Draw(12, 0);


	ID3D11ShaderResourceView * SRVNULL = nullptr;
	deviceContext->PSSetShaderResources(0, 1, &SRVNULL);

}

void Graphics::Renderer::createGUIBuffers()
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

void Graphics::Renderer::createBlendState()
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
	deviceContext->ClearRenderTargetView(backBuffer, clearColor);

	ID3D11ShaderResourceView * SRVS[] =
	{
		gbuffer.diffuseSpecView,
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

