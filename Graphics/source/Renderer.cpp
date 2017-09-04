#include "Renderer.h"
#include <stdio.h>
#include "ThrowIfFailed.h"

Renderer::Renderer(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
    : device(device)
    , deviceContext(deviceContext)
{
    createGBuffer();
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
    
    deviceContext->VSSetConstantBuffers(0, 1, nullptr);

    
    cull();
    draw();
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

    ID3D11Texture2D * diffuseSpecTexture;
    ID3D11Texture2D * normalMatTexture;
    ID3D11Texture2D * positionTexture;

    ThrowIfFailed(device->CreateTexture2D(&textureDesc, nullptr, &diffuseSpecTexture));
    ThrowIfFailed(device->CreateTexture2D(&textureDesc, nullptr, &normalMatTexture));
    ThrowIfFailed(device->CreateTexture2D(&textureDesc, nullptr, &positionTexture));
    ThrowIfFailed(device->CreateRenderTargetView(diffuseSpecTexture, nullptr, &gbuffer.diffuseSpec));
    ThrowIfFailed(device->CreateRenderTargetView(normalMatTexture, nullptr, &gbuffer.normalMat));
    ThrowIfFailed(device->CreateRenderTargetView(positionTexture, nullptr, &gbuffer.position));
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

