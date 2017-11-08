#include "SSAORenderPass.h"
#include "../Utility/TextureLoader.h"
#include "../MainCamera.h"

#define RANDOMNORMALTEXTURE TextureLoader::get().getTexture(Resources::Textures::randomNormals)

namespace Graphics 
{
    Graphics::SSAORenderPass::SSAORenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources,
        std::initializer_list<ID3D11Buffer*> buffers,
        ID3D11DepthStencilView * depthStencil, 
        std::initializer_list<ID3D11UnorderedAccessView*> uavs) :
        RenderPass(targets, resources, buffers, depthStencil),
        ssaoShader(Resources::Shaders::SSAOComputeShader),
        blurHorizontal(Resources::Shaders::SSAOGaussianBlurHorizontal),
        blurVertical(Resources::Shaders::SSAOGaussianBlurVertical),
        ssaoMerger(Resources::Shaders::SSAOMerger),
        ssaoOutput(WIN_WIDTH / 2, WIN_HEIGHT / 2, DXGI_FORMAT_R8_UNORM),
        ssaoOutputSwap(WIN_WIDTH / 2, WIN_HEIGHT / 2, DXGI_FORMAT_R8_UNORM),
        uavs(uavs)
    {
    }

    SSAORenderPass::~SSAORenderPass()
    {
    }

    void Graphics::SSAORenderPass::render() const
    {
        Global::context->OMSetRenderTargets(0, nullptr, nullptr);

        static float clear[4] = { 0 };
        Global::context->ClearUnorderedAccessViewFloat(ssaoOutput, clear);

        ID3D11SamplerState * samplers[] =
        {
            Global::cStates->PointClamp(),
            Global::cStates->PointWrap()
        };
        Global::context->CSSetSamplers(0, 2, samplers);

        Global::context->CSSetShader(ssaoShader, nullptr, 0);
        ID3D11ShaderResourceView * srvs[] =
        {
            resources[0],
            *RANDOMNORMALTEXTURE,
            resources[1]
        };

        Global::context->CSSetShaderResources(0, 3, srvs);
        Global::context->CSSetUnorderedAccessViews(0, 1, ssaoOutput, nullptr);
        Global::context->CSSetConstantBuffers(0, 1, *Global::mainCamera->getInverseBuffer());
        Global::context->Dispatch(40, 23, 1);

        ID3D11ShaderResourceView * srvsNULL[3] = { nullptr };
        Global::context->CSSetShaderResources(0, 3, srvsNULL);
        ID3D11ShaderResourceView * srvNULL = nullptr;

        //blur the occlusion map in two passes
        Global::context->CSSetShader(blurHorizontal, nullptr, 0);
        Global::context->CSSetUnorderedAccessViews(0, 1, ssaoOutputSwap, nullptr);
        Global::context->CSSetShaderResources(0, 1, ssaoOutput);
        Global::context->Dispatch(40, 23, 1);

        Global::context->CSSetShaderResources(0, 1, &srvNULL);

        Global::context->CSSetShader(blurVertical, nullptr, 0);
        Global::context->CSSetUnorderedAccessViews(0, 1, ssaoOutput, nullptr);
        Global::context->CSSetShaderResources(0, 1, ssaoOutputSwap);
        Global::context->Dispatch(40, 23, 1);

        //Merge the blurred occlusion map with back buffer
        Global::context->CSSetShader(ssaoMerger, nullptr, 0);
        Global::context->CSSetUnorderedAccessViews(0, 1, &uavs[0], nullptr);
        Global::context->CSSetShaderResources(0, 1, &resources[2]);
        Global::context->CSSetShaderResources(1, 1, ssaoOutput);
        Global::context->Dispatch(80, 45, 1);


        Global::context->CSSetShaderResources(0, 1, &srvNULL);
        Global::context->CSSetShaderResources(1, 1, &srvNULL);
        ID3D11UnorderedAccessView * uavnull = nullptr;
        Global::context->CSSetUnorderedAccessViews(0, 1, &uavnull, nullptr);
    }

    void Graphics::SSAORenderPass::update(float deltaTime)
    {
    }
}