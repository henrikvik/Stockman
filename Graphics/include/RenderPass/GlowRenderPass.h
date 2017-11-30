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
            PingPongBuffer * backBuffers,
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
        Shader glow;
        Shader glow2;
        Shader mipGenerator;
        Shader mipCombinder;

        ID3D11ShaderResourceView *m_BloomSRV;
        std::vector<ID3D11ShaderResourceView *> m_BloomSRVMipChain;
        std::vector<ID3D11RenderTargetView *> m_BloomRTVMipChain;

        ID3D11ShaderResourceView *glowtempSRV;
        ID3D11RenderTargetView * glowtempRTV;
        ID3D11ShaderResourceView * srvAllMips;

        ShaderResource glowPass0;
        ShaderResource glowPass1;

        PingPongBuffer * backBuffers;


        void createMips();
        void setMipViewPort(int level) const;
    };
}