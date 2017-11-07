#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"
#define SHADOW_RESOLUTION 2048

namespace Graphics {
    class ShadowRenderPass : public RenderPass
    {
    public:
        ShadowRenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr);
        virtual ~ShadowRenderPass() {};

        void update(float deltaTime);
        void render() const;

    private:
        D3D11_VIEWPORT viewport;
        D3D11_VIEWPORT viewportReset;
        Shader staticForwardPlus;

    };
}