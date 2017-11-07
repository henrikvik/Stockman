#include "DepthRenderPass.h"

namespace Graphics
{
    DepthRenderPass::DepthRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets, std::initializer_list<ID3D11ShaderResourceView*> resources, std::initializer_list<ID3D11Buffer*> buffers, ID3D11DepthStencilView * depthStencil)
        : RenderPass(targets, resources, buffers, depthStencil)
        , staticDepth(Resources::Shaders::ForwardPlus, ShaderType::VS)
    {
    }

    void DepthRenderPass::render() const
    {
        Global::context->IASetInputLayout(nullptr);
        Global::context->PSSetShader(nullptr, nullptr, 0);

        Global::context->OMSetRenderTargets(0, nullptr, depthStencil);
        Global::context->VSSetConstantBuffers(0, buffers.size(), buffers.data());

        Global::context->VSSetShader(staticDepth, nullptr, 0);
        drawInstanced<StaticRenderInfo>(resources[StaticInstanceBuffer]);
        // TODO add all renderInfos

        Global::context->OMSetRenderTargets(0, nullptr, nullptr);
        Global::context->VSSetConstantBuffers(0, buffers.size(), Global::nulls);
        Global::context->VSSetShader(nullptr, nullptr, 0);
    }

    void DepthRenderPass::update(float deltaTime)
    {
    }
}