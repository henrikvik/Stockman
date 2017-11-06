#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"

namespace Graphics
{
    class ForwardPlusRenderPass : public RenderPass
    {
    public:
        using RenderPass::RenderPass;

        virtual ~ForwardPlusRenderPass() {};

        // Inherited via RenderPass
        virtual void render() const override;
        virtual void update(float deltaTime) override;

    private:
        Shader staticForwardPlus;

        ID3D11RenderTargetView * colorTarget;
        ID3D11RenderTargetView * glowTarget;
        ID3D11RenderTargetView * normalsTarget;
        ID3D11DepthStencilView * depthStencil;

    };
}
