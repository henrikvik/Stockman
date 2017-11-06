#include "ForwardPlusRenderPass.h"
#include "../Device.h"
#include "../CommonStates.h"

namespace Graphics
{
    void ForwardPlusRenderPass::render() const
    {
        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(staticForwardPlus, nullptr, 0);
        Global::context->PSSetShader(staticForwardPlus, nullptr, 0);

        auto sampler = Global::cStates->LinearClamp();
        auto samplerWrap = Global::cStates->LinearWrap();
        Global::context->PSSetSamplers(0, 1, &sampler);
        Global::context->PSSetSamplers(1, 1, &Global::comparisonSampler);
        Global::context->PSSetSamplers(2, 1, &samplerWrap);

        Global::context->PSSetShaderResources(0, resources.size(), resources.data());


        ID3D11Buffer *lightBuffs[] =
        {
            *skyRenderer.getShaderBuffer(),
            *skyRenderer.getLightMatrixBuffer()
        };

        Global::context->PSSetConstantBuffers(1, 1, &lightBuffs[0]);
        Global::context->VSSetConstantBuffers(3, 1, &lightBuffs[1]);

        Global::context->OMSetRenderTargets(targets.size(), targets.data(), depthStencil);
    }

    void ForwardPlusRenderPass::update(float deltaTime)
    {
    }
}