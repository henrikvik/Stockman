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
        , depth_vs_static(Resources::Shaders::ForwardPlus_VS_Static, ShaderType::VS, HybrisLoader::Vertex::INPUT_DESC)
        , depth_vs_animated(Resources::Shaders::ForwardPlus_VS_Animated, ShaderType::VS, HybrisLoader::Vertex::INPUT_DESC)
        , depth_vs_foliage(Resources::Shaders::ForwardPlus_VS_Foliage, ShaderType::VS, HybrisLoader::Vertex::INPUT_DESC)
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

        Global::context->IASetInputLayout(depth_vs_static);
        Global::context->VSSetShader(depth_vs_static, nullptr, 0);
        drawInstanced<StaticRenderInfo>(resources[staticInstanceBuffer]);

        Global::context->IASetInputLayout(depth_vs_animated);
        Global::context->VSSetShader(depth_vs_animated, nullptr, 0);
        drawInstancedAnimated<AnimatedRenderInfo>(resources[animatedInstanceBuffer], resources[animatedJointsBuffer]);
        drawInstancedAnimated<NewAnimatedRenderInfo>(resources[newAnimatedInstanceBuffer], resources[newAnimatedJointsBuffer]);

        Global::context->RSSetState(Global::cStates->CullNone());
        Global::context->IASetInputLayout(depth_vs_foliage);
        Global::context->VSSetShader(depth_vs_foliage, nullptr, 0);
        Global::context->VSSetConstantBuffers(3, 1, &buffers[1]);
        drawInstanced<FoliageRenderInfo>(resources[foliageInstanceBuffer]);

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