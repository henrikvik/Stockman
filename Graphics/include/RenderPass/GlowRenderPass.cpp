#include "GlowRenderPass.h"
namespace Graphics
{
    GlowRenderPass::GlowRenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources,
        std::initializer_list<ID3D11Buffer*> buffers,
        ID3D11DepthStencilView * depthStencil) :
        RenderPass(targets, resources, buffers, depthStencil),
        glowMipCombinder(Resources::Shaders::)
    {
        
    }
    void GlowRenderPass::update(float deltaTime)
    {
    }

    void GlowRenderPass::render() const
    {
    }
}