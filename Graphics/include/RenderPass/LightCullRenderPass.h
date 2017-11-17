#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Lights/LightGrid.h"

namespace Graphics
{
    class LightCullRenderPass : public RenderPass
    {
    public:
        LightCullRenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources,
            std::initializer_list<ID3D11Buffer*> buffers,
            ID3D11DepthStencilView * depthStencil,
            std::initializer_list<ID3D11UnorderedAccessView*> uavs
        );

        virtual wchar_t* name() const override {
            return L"LightCullRenderPass";
        }

        // Inherited via RenderPass
        virtual void render() const override;
        virtual void update(float deltaTime) override;

    private:
        const std::vector<ID3D11UnorderedAccessView*> uavs;
        LightGrid lightGrid;
    };
}