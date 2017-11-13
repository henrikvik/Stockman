#pragma once
#include "RenderPass.h"

namespace Graphics
{
    class ParticleDepthRenderPass : public RenderPass
    {
    public:
        ParticleDepthRenderPass(ID3D11DepthStencilView * depthStencil);
        virtual ~ParticleDepthRenderPass() {};

        virtual void render() const override;
        virtual void update(float deltaTime) override;
    };

    class ParticleRenderPass : public RenderPass
    {
    public:
        ParticleRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets, ID3D11DepthStencilView * depthStencil);
        virtual ~ParticleRenderPass() {};

        virtual void render() const override;
        virtual void update(float deltaTime) override;
    };
}
