#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"

namespace Graphics 
{
    class FullScreenPass : public RenderPass
    {
    public:
        FullScreenPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr);
        virtual ~FullScreenPass() {};

        void update(float deltaTime) {};
        void render() const;

    private:
        Shader fullScreenShader;
    };
}