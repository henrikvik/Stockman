#include "ForwardPlusRenderPass.h"
#include "../Device.h"
#include "../CommonState.h"
#include "../RenderQueue.h"
#include "../HybrisLoader/HybrisLoader.h"

namespace Graphics
{
    ForwardPlusRenderPass::ForwardPlusRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets, std::initializer_list<ID3D11ShaderResourceView*> resources, std::initializer_list<ID3D11Buffer*> buffers, ID3D11DepthStencilView * depthStencil)
        : RenderPass(targets, resources, buffers, depthStencil)
        , forward_plus_ps(Resources::Shaders::ForwardPlus_PS, ShaderType::PS)
        , forward_plus_vs_static(Resources::Shaders::ForwardPlus_VS_Static, ShaderType::VS)
        , forward_plus_vs_animated(Resources::Shaders::ForwardPlus_VS_Animated, ShaderType::VS)
        , forward_plus_vs_foliage(Resources::Shaders::ForwardPlus_VS_Foliage, ShaderType::VS)
    {
    }

    void ForwardPlusRenderPass::render() const
    {
        PROFILE_BEGIN("Forward Plus");
        Global::context->IASetInputLayout(nullptr);
        Global::context->PSSetShader(forward_plus_ps, nullptr, 0);

        Global::context->VSSetConstantBuffers(0, buffers.size(), buffers.data());

        ID3D11SamplerState * samplers[] =
        {
            Global::cStates->LinearClamp(),
            Global::cStates->LinearWrap(),
            Global::comparisonSampler,
        };
        
        Global::context->PSSetSamplers(0, 3, samplers);
        Global::context->PSSetConstantBuffers(0, buffers.size(), buffers.data());
        Global::context->PSSetShaderResources(0, 4, resources.data());
        Global::context->PSSetShaderResources(4, 1, *TextureLoader::get().getTexture(Resources::Textures::Grid));

        Global::context->RSSetState(Global::cStates->CullClockwise());

        Global::context->OMSetDepthStencilState(Global::cStates->DepthRead(), 0x0);
        Global::context->OMSetRenderTargets(targets.size(), targets.data(), depthStencil);

        Global::context->RSSetState(Global::cStates->CullClockwise());
        Global::context->VSSetShader(forward_plus_vs_static, nullptr, 0);
        drawInstanced<StaticRenderInfo>(resources[4]);

        Global::context->RSSetState(Global::cStates->CullNone());
        Global::context->VSSetConstantBuffers(1, 1, &buffers[2]);
        Global::context->VSSetShader(forward_plus_vs_foliage, nullptr, 0);
        drawInstanced<FoliageRenderInfo>(resources[7]);


        Global::context->VSSetShader(forward_plus_vs_animated, nullptr, 0);
        drawInstancedAnimated<AnimatedRenderInfo>(resources[5], resources[6]);

        Global::context->PSSetSamplers(0, 3, Global::nulls);
        Global::context->OMSetRenderTargets(targets.size(), Global::nulls, nullptr);
        Global::context->PSSetShaderResources(0, 4, Global::nulls);
        Global::context->PSSetShaderResources(9, 1, Global::nulls);
        Global::context->PSSetConstantBuffers(0, buffers.size(), Global::nulls);
        Global::context->VSSetConstantBuffers(0, buffers.size(), Global::nulls);
        PROFILE_END();
    }

    void ForwardPlusRenderPass::update(float deltaTime)
    {
    }

    
}