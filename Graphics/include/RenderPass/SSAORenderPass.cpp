#include "SSAORenderPass.h"
#include "../Utility/TextureLoader.h"

#define RANDOMNORMALTEXTURE TextureLoader::get().getTexture(Resources::Textures::randomNormals)

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
        ssaoMerger(Resources::Shaders::SSAOMerger),
        ssaoOutput(WIN_WIDTH / 2, WIN_HEIGHT / 2, DXGI_FORMAT_R8_UNORM),
        ssaoOutputSwap(WIN_WIDTH / 2, WIN_HEIGHT / 2, DXGI_FORMAT_R8_UNORM)
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