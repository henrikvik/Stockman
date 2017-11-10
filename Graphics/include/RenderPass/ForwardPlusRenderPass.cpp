#include "ForwardPlusRenderPass.h"
#include "../Device.h"
#include "../CommonState.h"
#include "../RenderQueue.h"
#include "../HybrisLoader/HybrisLoader.h"

namespace Graphics
{
    ForwardPlusRenderPass::ForwardPlusRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets, std::initializer_list<ID3D11ShaderResourceView*> resources, std::initializer_list<ID3D11Buffer*> buffers, ID3D11DepthStencilView * depthStencil)
        : RenderPass(targets, resources, buffers, depthStencil)
        , staticForwardPlus(Resources::Shaders::ForwardPlus)
    {
    }

    void ForwardPlusRenderPass::render() const
    {
        PROFILE_BEGIN("Forward Plus");
        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(staticForwardPlus, nullptr, 0);
        Global::context->PSSetShader(staticForwardPlus, nullptr, 0);

        Global::context->PSSetConstantBuffers(0, buffers.size(), buffers.data());
        Global::context->VSSetConstantBuffers(0, buffers.size(), buffers.data());

        ID3D11SamplerState * samplers[] =
        {
            Global::cStates->LinearClamp(),
            Global::cStates->LinearWrap(),
            Global::comparisonSampler,
        };
        Global::context->PSSetSamplers(0, 3, samplers);



        Global::context->PSSetShaderResources(0, 4, resources.data());
        Global::context->PSSetShaderResources(9, 1, *TextureLoader::get().getTexture(Resources::Textures::Grid));
        Global::context->OMSetRenderTargets(targets.size(), targets.data(), depthStencil);


        drawInstanced<StaticRenderInfo>(resources[4]);
        // TODO add all renderInfos



        Global::context->PSSetSamplers(0, 3, Global::nulls);
        Global::context->OMSetRenderTargets(targets.size(), Global::nulls, nullptr);
        Global::context->PSSetShaderResources(0, 4, Global::nulls);
        Global::context->PSSetConstantBuffers(0, buffers.size(), Global::nulls);
        Global::context->VSSetConstantBuffers(0, buffers.size(), Global::nulls);
        PROFILE_END();
    }

    void ForwardPlusRenderPass::update(float deltaTime)
    {
    }

    
}