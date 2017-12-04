#pragma once

#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Utility/ShaderResource.h"
#include "../Utility/PingPongBuffer.h"

namespace Graphics
{
    class AORenderPass : public RenderPass
    {
    public:
        AORenderPass(
            ID3D11RenderTargetView *output,
            ID3D11ShaderResourceView *position,
            ID3D11ShaderResourceView *normal
        );
        virtual ~AORenderPass();

        virtual wchar_t* name() const override {
            return L"AORenderPass";
        }

        void render() const;
        void update(float deltaTime);

    private:
        struct AOConstants {
            DirectX::SimpleMath::Matrix Projection;
            DirectX::SimpleMath::Matrix View;
            DirectX::SimpleMath::Vector4 Kernel[32];
            float NoiseScale[4];
        } m_Constants;

        ConstantBuffer<AOConstants, 1> m_ConstantsBuffer;

        Shader m_SSAOPass;

        ID3D11RenderTargetView *m_Output;
        ID3D11ShaderResourceView *m_Position;
        ID3D11ShaderResourceView *m_Normal;
        ID3D11ShaderResourceView *m_Random;
    };
}