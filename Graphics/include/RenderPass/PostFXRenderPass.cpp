#include "PostFXRenderPass.h"
#include "../Device.h"
#include "../CommonState.h"
#include <algorithm>

namespace Graphics
{

    PostFXRenderPass::PostFXRenderPass(
        PingPongBuffer *backBuffers,
        ID3D11RenderTargetView *target,
        ID3D11ShaderResourceView *bloomSRV
    ) :
        RenderPass({}, {}, {}, nullptr),
        m_BloomSRV(bloomSRV),
        m_Target(target),
        m_ConstantsBuffer(),
        m_PostFXShader(Resources::Shaders::PostFX),
        backBuffers(backBuffers),
        m_Constants({})
    {
        m_ConstantsBuffer.write(Global::context, &m_Constants, sizeof(m_Constants));
    }

    void PostFXRenderPass::update(float deltaTime)
    {
        m_Constants = {};

        for (auto & info : RenderQueue::get().getQueue<SpecialEffectRenderInfo>()) {
            if (info.type == info.Tint) {
                m_Constants.m_Tint += info.color;

                if (info.progress <= 1.f && info.progress >= 0.f) {
                    m_Constants.m_TintProgress = info.progress;
                } else {
                    m_Constants.m_TintProgress = 0;
                }
            }

            if (info.type == info.BulletTime) {
                if (info.progress <= 1.f && info.progress >= 0.f) {
                    m_Constants.m_BulletTime = info.progress;
                } else {
                    m_Constants.m_BulletTime = 0;
                }
            }
        }

        //These two must always add up to one ir i'll have to fix the formula
        //They represents how long the fade in and fade out are. 
        static const float TOP_THRESHOLD = 0.9f;
        static const float BOT_THRESHOLD = 0.1f;

        // iNsAne C0d3
        if (m_Constants.m_BulletTime > TOP_THRESHOLD)
            m_Constants.m_BulletTime = (m_Constants.m_BulletTime - TOP_THRESHOLD) / BOT_THRESHOLD;
        else if (m_Constants.m_BulletTime < BOT_THRESHOLD)
            m_Constants.m_BulletTime = 1 - ((m_Constants.m_BulletTime) / BOT_THRESHOLD);
        else m_Constants.m_BulletTime = 0;

        m_Constants.m_BulletTime = std::clamp(m_Constants.m_BulletTime, 0.f, 1.f);


        m_ConstantsBuffer.write(Global::context, &m_Constants, sizeof(m_Constants));
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

        Global::context->PSSetConstantBuffers(0, 1, m_ConstantsBuffer);
        Global::context->PSSetShaderResources(0, 1, *backBuffers);
        Global::context->PSSetShaderResources(1, 1, &m_BloomSRV);

        auto sampler = Global::cStates->LinearClamp();
        Global::context->PSSetSamplers(0, 1, &sampler);
        Global::context->OMSetRenderTargets(1, &m_Target, nullptr);

        Global::context->Draw(3, 0);

        Global::context->OMSetRenderTargets(0, Global::nulls, nullptr);
        Global::context->PSSetShaderResources(0, 1, Global::nulls);
    }

}