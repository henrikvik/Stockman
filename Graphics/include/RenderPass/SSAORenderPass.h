#pragma once

#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Utility/ShaderResource.h"
#include "../Utility/PingPongBuffer.h"

namespace Graphics 
{
    class SSAORenderPass : public RenderPass
    {
    public:
        SSAORenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11UnorderedAccessView*> uavs = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr
            );
        virtual ~SSAORenderPass();

        virtual wchar_t* name() const override {
            return L"SSAORenderPass";
        }

        void render() const;
        void update(float deltaTime);

    private:
        ComputeShader ssaoShader;
        ComputeShader blurHorizontal;
        ComputeShader blurVertical;
        ComputeShader ssaoMerger;

        ShaderResource ssaoOutputSwap;

        std::vector<ID3D11UnorderedAccessView*> uavs;
    };
}