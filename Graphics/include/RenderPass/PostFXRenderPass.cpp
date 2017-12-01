#include "PostFXRenderPass.h"
#include "../Device.h"
#include "../CommonState.h"
#include <algorithm>

namespace Graphics
{

    PostFXRenderPass::PostFXRenderPass(
        PingPongBuffer *backBuffers,
        ID3D11RenderTargetView *target,
        ID3D11ShaderResourceView *bloomSRV,
        ID3D11ShaderResourceView *ssaoMap
    ) :
        RenderPass({}, {}, {}, nullptr),
        m_BloomSRV(bloomSRV),
        m_Target(target),
        m_BulletTimeBuffer(),
        m_PostFXShader(Resources::Shaders::PostFX),
        backBuffers(backBuffers),
        ssaoMap(ssaoMap)
    {
        float temp = 1.f;
        m_BulletTimeBuffer.write(Global::context, &temp, sizeof(float));
    }

    void PostFXRenderPass::update(float deltaTime)
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



        m_BulletTimeBuffer.write(Global::context, &progress, sizeof(float));
    }

    void PostFXRenderPass::render() const
    {
        
        backBuffers->swap();

        Global::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        Global::context->OMSetDepthStencilState(Global::cStates->DepthNone(), 0);
        Global::context->OMSetBlendState(Global::cStates->Opaque(), 0, -1);

        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(m_PostFXShader, nullptr, 0);
        Global::context->PSSetShader(m_PostFXShader, nullptr, 0);

        Global::context->PSSetConstantBuffers(0, 1, m_BulletTimeBuffer);
        Global::context->PSSetShaderResources(0, 1, *backBuffers);
        Global::context->PSSetShaderResources(1, 1, &m_BloomSRV);
        Global::context->PSSetShaderResources(2, 1, &ssaoMap);

        auto sampler = Global::cStates->LinearClamp();
        Global::context->PSSetSamplers(0, 1, &sampler);
        Global::context->OMSetRenderTargets(1, &m_Target, nullptr);

        Global::context->Draw(3, 0);

        Global::context->OMSetRenderTargets(0, Global::nulls, nullptr);
        Global::context->PSSetShaderResources(0, 1, Global::nulls);
    }

}