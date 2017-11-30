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
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr);
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
        Shader merger;
        Shader mipGenerator;
        Shader mipCombinder;

        ID3D11ShaderResourceView * srvs[MIP_LEVELS];
        ID3D11RenderTargetView * rtvs[MIP_LEVELS];

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