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
            ID3D11ShaderResourceView *depthSRV,
            ID3D11ShaderResourceView *(&viewSpaceDepthSRVSlices)[2],
            ID3D11RenderTargetView *(&viewSpaceDepthRTVSlices)[2]
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
            DirectX::SimpleMath::Vector2 NDCToViewMul;
            DirectX::SimpleMath::Vector2 NDCToViewAdd;
            DirectX::SimpleMath::Vector2 DepthUnpack;
            DirectX::SimpleMath::Vector2 CameraHalfFOV;
            DirectX::SimpleMath::Vector2 Viewport;
            DirectX::SimpleMath::Vector2 Viewport2X;
        } m_Constants;

        ConstantBuffer<AOConstants, 1> m_ConstantsBuffer;

        Shader m_SSAOPass;

        ID3D11ShaderResourceView *m_NormalSRV;
        ID3D11UnorderedAccessView *m_NormalUAV;

        ID3D11ShaderResourceView *m_DepthSRV;

        ID3D11ShaderResourceView *(&m_ViewSpaceDepthSRV)[2];
        ID3D11RenderTargetView *(&m_ViewSpaceDepthRTV)[2];

        ID3D11ShaderResourceView *m_Random;
    };
}