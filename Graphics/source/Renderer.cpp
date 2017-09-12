#include "Renderer.h"
#include <stdio.h>
#include "ThrowIfFailed.h"
#include <Engine\Constants.h>

namespace Graphics
{


Renderer::Renderer(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, ID3D11RenderTargetView * backBuffer)
    : simpleForward(gDevice, SHADER_PATH("SimpleForward.hlsl"), VERTEX_INSTANCE_DESC)
{
	this->device = gDevice;
	this->deviceContext = gDeviceContext;
	this->backBuffer = backBuffer;

    createGBuffer();
    createDepthStencil();
    createInstanceBuffer();
    initialize(gDevice, gDeviceContext);

    viewPort = { 0 };
    viewPort.Width = WIN_WIDTH;
    viewPort.Height = WIN_HEIGHT;
    viewPort.MaxDepth = 1.f;
}


Renderer::~Renderer()
{
    SAFE_RELEASE(instanceBuffer);
    SAFE_RELEASE(dSV);
    SAFE_RELEASE(dSS);
    gbuffer.Release();
}

void Renderer::initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext)
{
	resourceManager.initialize(gDevice, gDeviceContext);
}

void Renderer::render(Camera * camera)
{
    cull();

    ID3D11Buffer *cameraBuffer = camera->getBuffer();
    deviceContext->VSSetConstantBuffers(0, 1, &cameraBuffer);

    deviceContext->ClearDepthStencilView(gbuffer.depth, D3D11_CLEAR_DEPTH, 1, 0);
    deviceContext->OMSetRenderTargets(1, &backBuffer, gbuffer.depth);

    deviceContext->RSSetViewports(1, &viewPort);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    simpleForward.setShader(deviceContext);
    draw();
	
	////temp
	//this->drawDeffered();
	//this->drawToBackbuffer(gbuffer.positionView);
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

void Renderer::createDepthStencil()
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

void Renderer::createCubeInstances()
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

void Renderer::createInstanceBuffer()
{
    D3D11_BUFFER_DESC instanceDesc = { 0 };
    instanceDesc.ByteWidth      = sizeof(InstanceData) * INSTANCE_CAP;
    instanceDesc.Usage          = D3D11_USAGE_DYNAMIC;
    instanceDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    instanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    ThrowIfFailed(device->CreateBuffer(&instanceDesc, nullptr, &instanceBuffer));
}

void Renderer::draw()
{
	// Sort instance id from all meshes
	D3D11_MAPPED_SUBRESOURCE data = { 0 };

	deviceContext->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

    char* dest = (char*)data.pData;
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


	for (InstanceQueue_t::value_type & pair : instanceQueue)
	{
        ModelInfo model = resourceManager.getModelInfo(pair.first);

        buffers[0] = model.vertexBuffer;
        deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
        deviceContext->IASetIndexBuffer(model.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

        deviceContext->DrawIndexedInstanced((UINT)model.indexCount, (UINT)pair.second.size(), 0, 0, readOffset);
        readOffset += (UINT)pair.second.size() * sizeof(InstanceData);
	}

}

void Renderer::drawDeffered()
{

	//ID3D11RenderTargetView * RTVS[] =
	//{
	//	gbuffer.diffuseSpec,
	//	gbuffer.normalMat,
	//	gbuffer.position
	//};
	//float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//deviceContext->ClearRenderTargetView(gbuffer.diffuseSpec, clearColor);
	//deviceContext->ClearRenderTargetView(gbuffer.normalMat, clearColor);
	//deviceContext->ClearRenderTargetView(gbuffer.position, clearColor);
	//deviceContext->ClearDepthStencilView(dSV, D3D11_CLEAR_DEPTH, 1, 0);

	//deviceContext->OMSetRenderTargets(3, RTVS, dSV);
	//deviceContext->PSSetShaderResources(0, 1, &this->view);

	//resourceManager.setShaders(VertexShaderID::VERTEX_DEFFERED, PixelShaderID::PIXEL_DEFFERED, deviceContext);
	//resourceManager.setSampler(SamplerID::pointSampler, deviceContext);
	////textur här typ

	//UINT stride = 36, offset = 0;
	//UINT instanceStride = 12;
	//deviceContext->IASetVertexBuffers(0, 1, &defferedTestBuffer, &stride, &offset);
	//deviceContext->IASetVertexBuffers(1, 1, &instanceBuffer, &instanceStride, &offset);
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//deviceContext->DrawInstanced(36, 3, 0, 0);

	//ID3D11RenderTargetView * RTVNULLS[] =
	//{
	//	NULL,
	//	NULL,
	//	NULL
	//};
	//deviceContext->OMSetRenderTargets(3, RTVNULLS, NULL);
}

void Renderer::drawToBackbuffer(ID3D11ShaderResourceView * texture)
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

    UINT zero = 0;
    deviceContext->IASetVertexBuffers(0, 1, nullptr, &zero, &zero);
    deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, zero);
    deviceContext->IASetInputLayout(nullptr);
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

}
