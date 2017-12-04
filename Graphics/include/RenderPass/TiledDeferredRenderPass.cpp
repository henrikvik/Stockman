#include "TiledDeferredRenderPass.h"

#include "../MainCamera.h"

namespace Graphics {

    TiledDeferredRenderPass::TiledDeferredRenderPass(
        ID3D11RenderTargetView   *position,
        ID3D11RenderTargetView   *albedoSpec,
        ID3D11RenderTargetView   *normal,
        ID3D11ShaderResourceView *staticBuffer,
        ID3D11ShaderResourceView *animatedBuffer,
        ID3D11DepthStencilView   *depth
    )
        : m_Position(position), m_AlbedoSpec(albedoSpec), m_Normal(normal), m_StaticBuffer(staticBuffer),
         m_AnimatedBuffer(animatedBuffer), m_Depth(depth),
         m_TiledDeferredPS(Resources::Shaders::TiledDeferredPS, ShaderType::PS),
         m_TiledDeferredVS(Resources::Shaders::ForwardPlus_VS_Static, ShaderType::VS, HybrisLoader::Vertex::INPUT_DESC),
         m_TiledDeferredVSAnimated(Resources::Shaders::ForwardPlus_VS_Animated, ShaderType::VS, HybrisLoader::Vertex::INPUT_DESC),
        RenderPass({}, {}, {}, nullptr)
{
}

    void TiledDeferredRenderPass::render() const
    {
        auto cxt = Global::context;

        float CLEAR[4] = { 0.f, 0.f, 0.f, 0.f };
        cxt->ClearRenderTargetView(m_Position, CLEAR);
        cxt->ClearRenderTargetView(m_AlbedoSpec, CLEAR);
        cxt->ClearRenderTargetView(m_Normal, CLEAR);

        PROFILE_BEGIN("Tiled Deferred");
        cxt->IASetInputLayout(nullptr);
        cxt->PSSetShader(m_TiledDeferredPS, nullptr, 0);

        cxt->VSSetConstantBuffers(0, 1, *Global::mainCamera->getBuffer());
        cxt->PSSetConstantBuffers(0, 1, *Global::mainCamera->getBuffer());

        ID3D11SamplerState * samplers[] =
        {
            Global::cStates->LinearClamp(),
            Global::cStates->LinearWrap(),
            Global::comparisonSampler,
        };

        Global::context->PSSetSamplers(0, 3, samplers);
        cxt->PSSetShaderResources(4, 1, *TextureLoader::get().getTexture(Resources::Textures::Grid));

        cxt->RSSetState(Global::cStates->CullClockwise());

        ID3D11RenderTargetView *views[] = {
            m_Position,
            m_AlbedoSpec,
            m_Normal
        };
        cxt->OMSetDepthStencilState(Global::cStates->DepthRead(), 0x0);
        cxt->OMSetRenderTargets(3, views, m_Depth);

        cxt->IASetInputLayout(m_TiledDeferredVS);
        cxt->VSSetShader(m_TiledDeferredVS, nullptr, 0);
        drawInstanced<StaticRenderInfo>(m_StaticBuffer);

        cxt->IASetInputLayout(m_TiledDeferredVSAnimated);
        cxt->VSSetShader(m_TiledDeferredVSAnimated, nullptr, 0);
        drawInstanced<AnimatedRenderInfo>(m_AnimatedBuffer);

        cxt->PSSetSamplers(0, 4, Global::nulls);
        cxt->OMSetRenderTargets(targets.size(), Global::nulls, nullptr);
        cxt->PSSetShaderResources(0, 4, Global::nulls);
        cxt->PSSetShaderResources(9, 1, Global::nulls);
        cxt->PSSetConstantBuffers(0, buffers.size(), Global::nulls);
        cxt->VSSetConstantBuffers(0, buffers.size(), Global::nulls);
        PROFILE_END();
    }

    void TiledDeferredRenderPass::update(float deltaTime)
    {
    }

}
