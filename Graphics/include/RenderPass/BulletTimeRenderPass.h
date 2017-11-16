#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Utility/PingPongBuffer.h"

namespace Graphics
{
    class BulletTimeRenderPass : public RenderPass
    {
    public:
        BulletTimeRenderPass(
            PingPongBuffer * backBuffers,
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr);

        void update(float deltaTime);
        void render() const;

        ~BulletTimeRenderPass() {}


    private:
        ConstantBuffer<float> bulletTimeBuffer;
        Shader shader;
        PingPongBuffer * backBuffers;
    };
}