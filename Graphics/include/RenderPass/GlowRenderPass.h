#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Utility/ShaderResource.h"

#define MIP_LEVELS 5

namespace Graphics {
    class GlowRenderPass : public RenderPass
    {
    public:
        GlowRenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr);
        ~GlowRenderPass() {};

        void update(float deltaTime);
        void render() const;

    private:
        Shader glow;
        Shader glow2;
        Shader merger;
        Shader mipGenerator;
        Shader mipCombinder;

        ID3D11ShaderResourceView * srvs[MIP_LEVELS];
        ID3D11RenderTargetView * rtvs[MIP_LEVELS];

        ID3D11ShaderResourceView * srvAllMips;

        ShaderResource glowPass0;
        ShaderResource glowPass1;


        void createMips(ID3D11Device * device);
        void setMipViewPort(ID3D11DeviceContext * context, int level);
    };
}