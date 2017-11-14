#include "BulletTimeRenderPass.h"
#include "../Device.h"
#include "../CommonState.h"
#include <algorithm>

Graphics::BulletTimeRenderPass::BulletTimeRenderPass(
    PingPongBuffer * backBuffers,
    std::initializer_list<ID3D11RenderTargetView*> targets,
    std::initializer_list<ID3D11ShaderResourceView*> resources, 
    std::initializer_list<ID3D11Buffer*> buffers, 
    ID3D11DepthStencilView * depthStencil) :
    RenderPass(targets, resources, buffers, depthStencil), 
    bulletTimeBuffer(),
    shader(Resources::Shaders::BulletTime),
    backBuffers(backBuffers)
{
    float temp = 1.f;
    bulletTimeBuffer.write(Global::context, &temp, sizeof(float));
}

void Graphics::BulletTimeRenderPass::update(float deltaTime)
{
    static float progress = 0;
    int queuelength = 0;
    for (auto & info : RenderQueue::get().getQueue<SpecialEffectRenderInfo>())
    {
        if (info.type == info.BulletTime)
        {
            if (info.progress <= 1.f && info.progress >= 0.f)
            {
                progress = info.progress;
            }

            else
            {
                progress = 0;
            }

            queuelength++;
        }
    }

    if (queuelength == 0)
        progress = 0;

    //These two must always add up to one ir i'll have to fix the formula
    //They represents how long the fade in and fade out are. 
    static const float TOP_THRESHOLD = 0.9f;
    static const float BOT_THRESHOLD = 0.1f;


    if (progress > TOP_THRESHOLD)
        progress = (progress - TOP_THRESHOLD) / BOT_THRESHOLD;

    else if (progress < BOT_THRESHOLD)
        progress = 1 - ((progress) / BOT_THRESHOLD);

    else progress = 0;

    progress = std::clamp(progress, 0.f, 1.f);

   

    bulletTimeBuffer.write(Global::context, &progress, sizeof(float));
}

void Graphics::BulletTimeRenderPass::render() const
{
    backBuffers->swap();

    Global::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Global::context->OMSetDepthStencilState(Global::cStates->DepthNone(), 0);
    Global::context->OMSetBlendState(Global::cStates->Opaque(), 0, -1);

    Global::context->IASetInputLayout(nullptr);
    Global::context->VSSetShader(shader, nullptr, 0);
    Global::context->PSSetShader(shader, nullptr, 0);

    Global::context->PSSetConstantBuffers(0, 1, bulletTimeBuffer);
    Global::context->PSSetShaderResources(0, 1, *backBuffers);

    auto steste = Global::cStates->LinearClamp();
    Global::context->PSSetSamplers(0, 1, &steste);
    Global::context->OMSetRenderTargets(1, &targets[0], nullptr);

    Global::context->Draw(3, 0);

    Global::context->OMSetRenderTargets(0, Global::nulls, nullptr);
    Global::context->PSSetShaderResources(0, 1, Global::nulls);
}
