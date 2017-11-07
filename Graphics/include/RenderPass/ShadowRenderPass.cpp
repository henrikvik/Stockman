#include "ShadowRenderPass.h"
#include "../Device.h"
#include <Engine\Constants.h>
#define SHADOW_RESOLUTION 2048
namespace Graphics
{
    ShadowRenderPass::ShadowRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources = {},
        std::initializer_list<ID3D11Buffer*> buffers = {},
        ID3D11DepthStencilView * depthStencil = nullptr)
        : RenderPass(targets, resources, buffers, depthStencil),
        staticForwardPlus(Resources::Shaders::ForwardPlus, ShaderType::VS)
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
        Global::context->RSSetViewports(1, &viewport);
        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(staticForwardPlus, nullptr, 0);
        Global::context->PSSetShader(nullptr, nullptr, 0);
        Global::context->OMSetRenderTargets(0, nullptr, depthStencil);
        Global::context->VSSetConstantBuffers(0, 1, &buffers[0]);

        drawInstanced<StaticRenderInfo>(resources[0]);
            

        //reset
        Global::context->RSSetViewports(1, &viewportReset);
        Global::context->OMSetRenderTargets(0, nullptr, nullptr);
    }
}