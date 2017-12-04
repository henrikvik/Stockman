#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"

namespace Graphics
{
    class TiledDeferredLightingRenderPass : public RenderPass
    {
    public:
        TiledDeferredLightingRenderPass(
            ID3D11RenderTargetView   *output,
            ID3D11RenderTargetView   *glow,
            ID3D11ShaderResourceView *position,
            ID3D11ShaderResourceView *albedoSpec,
            ID3D11ShaderResourceView *normal,
            ID3D11ShaderResourceView *ao,
            ID3D11ShaderResourceView *lightGrid,
            ID3D11ShaderResourceView *lightIndexList,
            ID3D11ShaderResourceView *lights,
            ID3D11ShaderResourceView *shadowMap,
            ID3D11Buffer *lightBuffer,
            ID3D11Buffer *shadowBuffer,
            ID3D11DepthStencilView *depthDSV
        );
        virtual ~TiledDeferredLightingRenderPass() {};

        virtual wchar_t* name() const override {
            return L"TiledDeferredLightingRenderPass";
        }

        // Inherited via RenderPass
        virtual void render() const override;
        virtual void update(float deltaTime) override;

    private:
        ID3D11RenderTargetView *m_Output;
        ID3D11RenderTargetView *m_Glow;

        ID3D11ShaderResourceView *m_AlbedoSpec;
        ID3D11ShaderResourceView *m_Normal;
        ID3D11ShaderResourceView *m_Position;
        ID3D11ShaderResourceView *m_AO;

        ID3D11ShaderResourceView *m_AnimatedBuffer;

        ID3D11ShaderResourceView *m_LightGrid;
        ID3D11ShaderResourceView *m_LightIndexList;
        ID3D11ShaderResourceView *m_Lights;
        ID3D11ShaderResourceView *m_ShadowMap;

        ID3D11Buffer *m_LightBuffer;
        ID3D11Buffer *m_ShadowBuffer;

        ID3D11DepthStencilView *m_DepthDSV;

        Shader m_TiledDeferredLightingVS;
        Shader m_TiledDeferredLightingPS;
    };
}
