#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"

namespace Graphics
{
    class ForwardPlusRenderPass : public RenderPass
    {
    public:
        ForwardPlusRenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr
        );
        virtual ~ForwardPlusRenderPass() {};

        // Inherited via RenderPass
        virtual void render() const override;
        virtual void update(float deltaTime) override;

    private:
        Shader forward_plus_ps;
        Shader forward_plus_vs_static;
        Shader forward_plus_vs_animated;
    };
}
