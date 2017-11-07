#include "ParticleRenderPass.h"

#include "../Particles/ParticleSystem.h"
#include "../MainCamera.h"

namespace Graphics {

    Graphics::ParticleRenderPass::ParticleRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets, ID3D11DepthStencilView * depthStencil)
        : RenderPass(targets, {}, {}, depthStencil)
    {
    }

    void ParticleRenderPass::render() const
    {
        FXSystem->render(Global::context, Global::mainCamera, targets[0], depthStencil, false);
    }

    void ParticleRenderPass::update(float deltaTime)
    {
    }

    ParticleDepthRenderPass::ParticleDepthRenderPass(ID3D11DepthStencilView * depthStencil)
        : RenderPass({}, {}, {}, depthStencil)
    {
    }

    void ParticleDepthRenderPass::render() const
    {
        FXSystem->renderPrePass(Global::context, Global::mainCamera, depthStencil);
    }

    void ParticleDepthRenderPass::update(float deltaTime)
    {
    }

}