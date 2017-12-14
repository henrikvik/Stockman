#pragma once

#include "RenderPass.h"
#include "../Utility/Shader.h"
#include "../Utility/ShaderResource.h"
#include "../Utility/PingPongBuffer.h"

#include <array>

namespace Graphics
{
    class AORenderPass : public RenderPass
    {
    public:
        AORenderPass(
            ID3D11ShaderResourceView *depthSRV,
            std::vector<ID3D11ShaderResourceView *> aoSlicesSRV,
            std::vector<ID3D11RenderTargetView *> aoSlicesRTV
        );
        virtual ~AORenderPass();

        virtual wchar_t* name() const override {
            return L"AORenderPass";
        }

        void pass(int pass) const;
        void render() const;
        void update(float deltaTime);

    private:
        struct AOConstants {
            int PerPassFullResCoordOffset[2];
            DirectX::SimpleMath::Vector2 NDCToViewMul;
            DirectX::SimpleMath::Vector2 NDCToViewAdd;
            DirectX::SimpleMath::Vector2 DepthUnpack;
            DirectX::SimpleMath::Vector2 CameraHalfFOV;
            DirectX::SimpleMath::Vector2 HalfViewport;
            DirectX::SimpleMath::Vector2 Viewport;
            DirectX::SimpleMath::Vector2 Viewport2X;
            DirectX::SimpleMath::Vector2 Viewport2X025;
            DirectX::SimpleMath::Vector2 _Padding;
            DirectX::SimpleMath::Vector4 PatternRotScaleMatrices[5];
        } m_Constants;

        ConstantBuffer<AOConstants, 1> m_ConstantsBuffer;

        Shader m_SSAOPrepareDepthNormals;
        Shader m_SSAOGenerate;

        // owned:
        ID3D11SamplerState *m_PointMirror;

        ID3D11ShaderResourceView *m_NormalSRV;
        ID3D11UnorderedAccessView *m_NormalUAV;


        ID3D11ShaderResourceView *m_ViewSpaceDepthSRV[2];
        ID3D11RenderTargetView *m_ViewSpaceDepthRTV[2];

        // borrowed:
        ID3D11ShaderResourceView *m_DepthSRV;

        std::vector<ID3D11ShaderResourceView *> m_AOSlicesSRV;
        std::vector<ID3D11RenderTargetView *> m_AOSlicesRTV;

    };
}