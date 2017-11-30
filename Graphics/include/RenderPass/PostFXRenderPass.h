#pragma once
#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Utility/PingPongBuffer.h"

namespace Graphics
{
    class PostFXRenderPass : public RenderPass
    {
    public:
        PostFXRenderPass(
            PingPongBuffer *backBuffers,
            ID3D11RenderTargetView *target,
            ID3D11ShaderResourceView *bloomSRV
        );

        virtual wchar_t* name() const override {
            return L"PostFXRenderPass";
        }

        void update(float deltaTime);
        void render() const;

        ~PostFXRenderPass() {}


    private:
        Shader m_PostFXShader;
        PingPongBuffer * backBuffers;
        ConstantBuffer<float> m_BulletTimeBuffer;

        ID3D11ShaderResourceView *m_BloomSRV;
        ID3D11RenderTargetView *m_Target;
    };
}