#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Utility/PingPongBuffer.h"

#define MIP_LEVELS 4

namespace Graphics {
    class GlowRenderPass : public RenderPass
    {
    public:
        GlowRenderPass(
            ID3D11ShaderResourceView *bloomSRV,
            std::vector<ID3D11ShaderResourceView *> bloomSRVChain,
            std::vector<ID3D11RenderTargetView *> bloomRTVChain
        );
        virtual ~GlowRenderPass();

        virtual wchar_t* name() const override {
            return L"GlowRenderPass";
        }

        void update(float deltaTime);
        void render() const;

    private:
        Shader m_KawaseDualFilterDownsample;
        Shader m_KawaseDualFilterUpsample;
        Shader m_GaussianHorizontal;
        Shader m_GaussianVertical;

        ID3D11ShaderResourceView *m_BloomSRV;
        std::vector<ID3D11ShaderResourceView *> m_BloomSRVMipChain;
        std::vector<ID3D11RenderTargetView *> m_BloomRTVMipChain;

        ID3D11ShaderResourceView *m_BlurTempSRV;
        ID3D11RenderTargetView *m_BlurTempRTV;

        void createMips();
        void setMipViewPort(int level) const;
    };
}