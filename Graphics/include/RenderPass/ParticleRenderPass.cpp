#include "ParticleRenderPass.h"

#include "../Particles/ParticleSystem.h"
#include "../MainCamera.h"
#include <Engine\DebugWindow.h>

namespace Graphics {

    Graphics::ParticleRenderPass::ParticleRenderPass(
            ID3D11RenderTargetView *output,
            ID3D11ShaderResourceView *lightGrid,
            ID3D11ShaderResourceView *lightIndexList,
            ID3D11ShaderResourceView *lights,
            ID3D11ShaderResourceView *shadowMap,
            ID3D11Buffer *lightBuffer,
            ID3D11DepthStencilView *depthStencil)
        : RenderPass({}, {}, {}, depthStencil),
          m_Output(output),
          m_LightGrid(lightGrid),
          m_LightIndexList(lightIndexList),
          m_Lights(lights),
          m_LightBuffer(lightBuffer),
          m_ShadowMap(shadowMap)
    {
        
    }

    void ParticleRenderPass::render() const
    {
        FXSystem->render(Global::context,
            Global::mainCamera,
            m_LightIndexList,
            m_LightGrid,
            m_Lights,
            m_LightBuffer,
            m_ShadowMap,
            m_Output,
            depthStencil,
            false
        );
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
