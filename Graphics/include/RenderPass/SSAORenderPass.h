#pragma once

#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Utility/ShaderResource.h"

namespace Graphics 
{
    class SSAORenderPass : public RenderPass
    {
    public:
        SSAORenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr);
        virtual ~SSAORenderPass();

        void render() const;
        void update(float deltaTime);

    private:
        ComputeShader ssaoShader;
        ComputeShader blurHorizontal;
        ComputeShader blurVertical;
        ComputeShader ssaoMerger;

        ID3D11ShaderResourceView * randomNormals;
        ShaderResource ssaoOutput;
        ShaderResource ssaoOutputSwap;
    };
}