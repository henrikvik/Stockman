#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"

namespace Graphics
{
    class TiledDeferredRenderPass : public RenderPass
    {
    public:
        TiledDeferredRenderPass(
            ID3D11RenderTargetView* position,
            ID3D11RenderTargetView* albedoSpec,
            ID3D11RenderTargetView* normal,
            ID3D11ShaderResourceView *staticBuffer,
            ID3D11ShaderResourceView *animatedBuffer,
            ID3D11DepthStencilView *depth
        );
        virtual ~TiledDeferredRenderPass() {};

        virtual wchar_t* name() const override {
            return L"TiledDeferredRenderPass";
        }

        // Inherited via RenderPass
        virtual void render() const override;
        virtual void update(float deltaTime) override;

    private:
        ID3D11RenderTargetView   *m_Position;
        ID3D11RenderTargetView   *m_AlbedoSpec;
        ID3D11RenderTargetView   *m_Normal;
        ID3D11ShaderResourceView *m_StaticBuffer;
        ID3D11ShaderResourceView *m_AnimatedBuffer;
        ID3D11DepthStencilView   *m_Depth;

        Shader m_TiledDeferredPS;
        Shader m_TiledDeferredVS;
        Shader m_TiledDeferredVSAnimated;
    };
}
