#include "ShadowRenderPass.h"
#include "../Device.h"
#include <Engine\Constants.h>
#include "../CommonState.h"
namespace Graphics
{
    ShadowRenderPass::ShadowRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources,
        std::initializer_list<ID3D11Buffer*> buffers,
        ID3D11DepthStencilView * depthStencil)
        : RenderPass(targets, resources, buffers, depthStencil)
        , forward_plus_vs_static(Resources::Shaders::ForwardPlus_VS_Static, ShaderType::VS, HybrisLoader::Vertex::INPUT_DESC)
        , forward_plus_vs_animated(Resources::Shaders::ForwardPlus_VS_Animated, ShaderType::VS, HybrisLoader::Vertex::INPUT_DESC)
    {
        viewport = { 0 };
        viewport.Height = SHADOW_RESOLUTION;
        viewport.Width = SHADOW_RESOLUTION;
        viewport.MaxDepth = 1.f;

        viewportReset = viewport;
        viewportReset.Width = WIN_WIDTH;
        viewportReset.Height = WIN_HEIGHT;
    }

    void ShadowRenderPass::update(float deltaTime)
    {
    }

    void ShadowRenderPass::render() const
    {
        PROFILE_BEGIN("Shadow");
        Global::context->OMSetDepthStencilState(Global::cStates->DepthDefault(), 0);
        Global::context->RSSetViewports(1, &viewport);
        Global::context->IASetInputLayout(nullptr);
        Global::context->GSSetShader(nullptr, nullptr, 0);
        Global::context->PSSetShader(nullptr, nullptr, 0);
        
        Global::context->RSSetState(Global::cStates->CullCounterClockwise());
        Global::context->OMSetRenderTargets(0, nullptr, depthStencil);
        Global::context->VSSetConstantBuffers(0, 1, &buffers[0]);

        Global::context->IASetInputLayout(forward_plus_vs_static);
        Global::context->VSSetShader(forward_plus_vs_static, nullptr, 0);
        drawInstanced<StaticRenderInfo>(resources[StaticInstanceBuffer]);
        
        Global::context->IASetInputLayout(forward_plus_vs_animated);
        Global::context->VSSetShader(forward_plus_vs_animated, nullptr, 0);
        drawInstanced<AnimatedRenderInfo>(resources[AnimatedInstanceBuffer]);


        //reset
        Global::context->RSSetViewports(1, &viewportReset);
        Global::context->RSSetState(Global::cStates->CullClockwise());
        Global::context->OMSetRenderTargets(0, nullptr, nullptr);
        PROFILE_END();
    }
}