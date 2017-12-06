#include "SSAORenderPass.h"
#include "../Utility/TextureLoader.h"
#include "../MainCamera.h"
#include <Singeltons\DebugWindow.h>

#define RANDOMNORMALTEXTURE TextureLoader::get().getTexture(Resources::Textures::randomNormals)

namespace Graphics 
{
    Graphics::SSAORenderPass::SSAORenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources,
        std::initializer_list<ID3D11UnorderedAccessView*> uavs,
        std::initializer_list<ID3D11Buffer*> buffers,
        ID3D11DepthStencilView * depthStencil) :
        RenderPass(targets, resources, buffers, depthStencil),
        ssaoShader(Resources::Shaders::SSAOComputeShader),
        blurHorizontal(Resources::Shaders::SSAOGaussianBlurHorizontal),
        blurVertical(Resources::Shaders::SSAOGaussianBlurVertical),
        ssaoMerger(Resources::Shaders::SSAOMerger),
        ssaoOutputSwap(WIN_WIDTH / 2, WIN_HEIGHT / 2, DXGI_FORMAT_R8_UNORM),
        uavs(uavs)
    {
        DebugWindow::getInstance()->registerCommand("GFX_SET_SSAO", [&](std::vector<std::string> &args)->std::string
        {
            std::string catcher = "";
            try
            {
                if (args.size() != 0)
                {
                    enabled = std::stoi(args[0]);

                    if (enabled)
                        catcher = "SSAO enabled!";

                    else
                        catcher = "SSAO disabled!";
                }
                else
                {
                    catcher = "missing argument 0 or 1.";
                }
            }
            catch (const std::exception&)
            {
                catcher = "Argument must be 0 or 1.";
            }

            return catcher;
        });
    }

    SSAORenderPass::~SSAORenderPass()
    {
    }

    void Graphics::SSAORenderPass::render() const
    {
        if (!enabled)
            return;

        PROFILE_BEGIN("SSAO");


        Global::context->OMSetRenderTargets(0, nullptr, nullptr);

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
        Global::context->CSSetUnorderedAccessViews(0, 1, &uavs[0], nullptr);
        Global::context->CSSetConstantBuffers(0, 1, *Global::mainCamera->getInverseBuffer());
        Global::context->Dispatch(40, 23, 1);

        Global::context->CSSetShaderResources(0, 3, Global::nulls);

        //blur the occlusion map in two passes
        Global::context->CSSetShader(blurHorizontal, nullptr, 0);
        Global::context->CSSetUnorderedAccessViews(0, 1, ssaoOutputSwap, nullptr);
        Global::context->CSSetShaderResources(0, 1, &resources[2]);
        Global::context->Dispatch(40, 23, 1);

        Global::context->CSSetShaderResources(0, 1, Global::nulls);

        Global::context->CSSetShader(blurVertical, nullptr, 0);
        Global::context->CSSetUnorderedAccessViews(0, 1, &uavs[0], nullptr);
        Global::context->CSSetShaderResources(0, 1, ssaoOutputSwap);
        Global::context->Dispatch(40, 23, 1);

        Global::context->CSSetShaderResources(0, 2, Global::nulls);
        Global::context->CSSetUnorderedAccessViews(0, 1, Global::nulls, nullptr);
        PROFILE_END();
    }

    void Graphics::SSAORenderPass::update(float deltaTime)
    {
    }
}