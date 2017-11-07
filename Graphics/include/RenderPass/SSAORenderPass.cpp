#include "SSAORenderPass.h"
namespace Graphics 
{
    Graphics::SSAORenderPass::SSAORenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources,
        std::initializer_list<ID3D11Buffer*> buffers,
        ID3D11DepthStencilView * depthStencil) :
        RenderPass(targets, resources, buffers, depthStencil),
        ssaoShader(Resources::Shaders::SSAOComputeShader),
        blurHorizontal(Resources::Shaders::SSAOGaussianBlurHorizontal),
        blurVertical(Resources::Shaders::SSAOGaussianBlurVertical),
        ssaoMerger(Resources::Shaders::SSAOMerger)
    {
    }

    SSAORenderPass::~SSAORenderPass()
    {
    }

    void Graphics::SSAORenderPass::render() const
    {
    }

    void Graphics::SSAORenderPass::update(float deltaTime)
    {
    }
}