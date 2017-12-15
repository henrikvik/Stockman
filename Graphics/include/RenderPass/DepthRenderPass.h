#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"

namespace Graphics
{
    class DepthRenderPass : public RenderPass
    {
    public:
        enum BufferSlots { Camera };
        enum ResourceSlots { 
            staticInstanceBuffer, 
            animatedInstanceBuffer, 
            animatedJointsBuffer,
            foliageInstanceBuffer,
            newAnimatedInstanceBuffer,
            newAnimatedJointsBuffer,
        };

        DepthRenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr
        );

        virtual wchar_t* name() const override {
            return L"DepthRenderPass";
        }

        // Inherited via RenderPass
        virtual void render() const override;
        virtual void update(float deltaTime) override;

    private:
        Shader depth_vs_static;
        Shader depth_vs_animated;
        Shader depth_vs_foliage;
        Shader depth_ps_foliage;

    };
}