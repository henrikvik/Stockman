#include "ParticleRenderPass.h"

#include "../Particles/ParticleSystem.h"
#include "../MainCamera.h"
#include <Engine\DebugWindow.h>

namespace Graphics {

    Graphics::ParticleRenderPass::ParticleRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets, ID3D11DepthStencilView * depthStencil)
        : RenderPass(targets, {}, {}, depthStencil)
    {
        
    }

    void ParticleRenderPass::render() const
    {
        PROFILE_BEGIN("Particle");
        FXSystem->render(Global::context, Global::mainCamera, targets[0], depthStencil, false);
        PROFILE_END();

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