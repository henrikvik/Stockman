#include "TiledDeferredLightingRenderPass.h"

#include "../MainCamera.h"

namespace Graphics {

TiledDeferredLightingRenderPass::TiledDeferredLightingRenderPass(
    ID3D11RenderTargetView   *output,
    ID3D11ShaderResourceView *position,
    ID3D11ShaderResourceView *albedoSpec,
    ID3D11ShaderResourceView *normal,
    ID3D11ShaderResourceView *lightGrid,
    ID3D11ShaderResourceView *lightIndexList,
    ID3D11ShaderResourceView *lights,
    ID3D11ShaderResourceView *shadowMap,
    ID3D11Buffer *lightBuffer
)
    : m_Output(output), m_AlbedoSpec(albedoSpec), m_Normal(normal), m_Position(position),
      m_LightGrid(lightGrid), m_LightIndexList(lightIndexList), m_Lights(lights),
      m_ShadowMap(shadowMap), m_LightBuffer(lightBuffer), RenderPass({}, {}, {}, nullptr),
      m_TiledDeferredLightingVS(Resources::Shaders::TiledDeferredLighting, ShaderType::VS),
      m_TiledDeferredLightingPS(Resources::Shaders::TiledDeferredLighting, ShaderType::PS)
{
}

void TiledDeferredLightingRenderPass::render() const
{
    auto cxt = Global::context;

    PROFILE_BEGIN("Tiled Deferred");
    cxt->IASetInputLayout(nullptr);
    cxt->VSSetShader(m_TiledDeferredLightingVS, nullptr, 0);
    cxt->PSSetShader(m_TiledDeferredLightingPS, nullptr, 0);

    ID3D11SamplerState *samplers[] = {
        Global::cStates->LinearClamp(),
        Global::cStates->LinearWrap(),
    };

    ID3D11Buffer *buffers[] = {
        *Global::mainCamera->getBuffer(),
        m_LightBuffer
    };
    cxt->PSSetConstantBuffers(0, 2, buffers);
    cxt->PSSetSamplers(0, 2, samplers);
    ID3D11ShaderResourceView *views[] = {
        m_LightIndexList,
        m_LightGrid,
        m_Lights,
        m_Position,
        m_AlbedoSpec,
        m_Normal,
        m_ShadowMap
    };
    cxt->PSSetShaderResources(0, 7, views);
    cxt->RSSetState(Global::cStates->CullNone());
    cxt->OMSetRenderTargets(1, &m_Output, nullptr);

    cxt->Draw(3, 0);

    cxt->PSSetSamplers(0, 3, Global::nulls);
    cxt->OMSetRenderTargets(1, Global::nulls, nullptr);
    cxt->PSSetShaderResources(0, 15, Global::nulls);
    PROFILE_END();
}

void TiledDeferredLightingRenderPass::update(float deltaTime)
{
}

}
