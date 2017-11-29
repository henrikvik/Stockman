#include "DepthRenderPass.h"

namespace Graphics
{
    DepthRenderPass::DepthRenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources,
        std::initializer_list<ID3D11Buffer*> buffers,
        ID3D11DepthStencilView * depthStencil
    )
        : RenderPass(targets, resources, buffers, depthStencil)
        , depth_vs_static(Resources::Shaders::ForwardPlus_VS_Static, ShaderType::VS)
        , depth_vs_animated(Resources::Shaders::ForwardPlus_VS_Animated, ShaderType::VS)
    {
    }

    void DepthRenderPass::render() const
    {
        PROFILE_BEGIN("Depth Pass");
        Global::context->OMSetDepthStencilState(Global::cStates->DepthDefault(), 0);
        Global::context->IASetInputLayout(nullptr);
        Global::context->PSSetShader(nullptr, nullptr, 0);

        Global::context->RSSetState(Global::cStates->CullClockwise());

        Global::context->OMSetRenderTargets(0, nullptr, depthStencil);
        Global::context->VSSetConstantBuffers(0, buffers.size(), buffers.data());

        Global::context->VSSetShader(depth_vs_static, nullptr, 0);
        drawInstanced<StaticRenderInfo>(resources[StaticInstanceBuffer]);

        Global::context->VSSetShader(depth_vs_animated, nullptr, 0);
        drawInstancedAnimated<AnimatedRenderInfo>(resources[AnimatedInstanceBuffer], resources[AnimatedInstanceBuffer + 1]);

        // TODO add all renderInfos

        Global::context->OMSetRenderTargets(0, nullptr, nullptr);
        Global::context->VSSetConstantBuffers(0, buffers.size(), Global::nulls);
        Global::context->VSSetShader(nullptr, nullptr, 0);
        PROFILE_END();
    }

    void DepthRenderPass::update(float deltaTime)
    {
    }
}