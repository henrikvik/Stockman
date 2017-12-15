#include "AORenderPass.h"

#include <random>

namespace Graphics
{

#define KERNEL_SIZE 16

    AORenderPass::AORenderPass(
        ID3D11ShaderResourceView *depthSRV,
        std::vector<ID3D11ShaderResourceView *> aoSlicesSRV,
        std::vector<ID3D11RenderTargetView *> aoSlicesRTV
    )
        : RenderPass({}, {}, {}, nullptr),
          m_SSAOPrepareDepthNormals(Resources::Shaders::SSAO),
          m_SSAOGenerate(Resources::Shaders::SSAOGenerate, ShaderType::PS),
          m_SSAOBlur(Resources::Shaders::SSAOBlur, ShaderType::PS),
          m_DepthSRV(depthSRV),
          m_AOSlicesSRV(aoSlicesSRV),
          m_AOSlicesRTV(aoSlicesRTV)
    {
        pass(0);

        auto device = Global::device;

        {
            D3D11_TEXTURE2D_DESC desc = {};
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
            desc.Width = WIN_WIDTH;
            desc.Height = WIN_HEIGHT;
            desc.SampleDesc.Count = 1;
            desc.MipLevels = 1;
            desc.ArraySize = 1;


            D3D11_UNORDERED_ACCESS_VIEW_DESC uav = {};
            uav.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            uav.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
            uav.Texture2D.MipSlice = 0;

            ID3D11Texture2D *texture = nullptr;
            ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &texture));
            ThrowIfFailed(device->CreateShaderResourceView(texture, nullptr, &m_NormalSRV));
            ThrowIfFailed(device->CreateUnorderedAccessView(texture, &uav, &m_NormalUAV));
            SAFE_RELEASE(texture);
        }

        {
            D3D11_TEXTURE2D_DESC desc = {};
            desc.Format = DXGI_FORMAT_R16_FLOAT;
            desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            desc.Width = WIN_WIDTH / 2;
            desc.Height = WIN_HEIGHT / 2;
            desc.SampleDesc.Count = 1;
            desc.MipLevels = 1;
            desc.ArraySize = 1;


            for (int i = 0; i < 2; i++) {
                ID3D11Texture2D *texture = nullptr;

                ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &texture));
                ThrowIfFailed(device->CreateShaderResourceView(texture, nullptr, &m_ViewSpaceDepthSRV[i]));
                ThrowIfFailed(device->CreateRenderTargetView(texture, nullptr, &m_ViewSpaceDepthRTV[i]));
                SAFE_RELEASE(texture);
            }
        }

        {
            D3D11_TEXTURE2D_DESC desc = {};
            desc.Format = DXGI_FORMAT_R8_UNORM;
            desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            desc.Width = WIN_WIDTH / 2;
            desc.Height = WIN_HEIGHT / 2;
            desc.SampleDesc.Count = 1;
            desc.MipLevels = 1;
            desc.ArraySize = 1;


            for (int i = 0; i < 2; i++) {
                ID3D11Texture2D *texture = nullptr;

                ThrowIfFailed(device->CreateTexture2D(&desc, nullptr, &texture));
                ThrowIfFailed(device->CreateShaderResourceView(texture, nullptr, &m_BlurIntermediateSRV[i]));
                ThrowIfFailed(device->CreateRenderTargetView(texture, nullptr, &m_BlurIntermediateRTV[i]));
                SAFE_RELEASE(texture);
            }
        }

        {
            D3D11_SAMPLER_DESC desc = {};
            desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
            desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
            desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
            desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

            ThrowIfFailed(device->CreateSamplerState(&desc, &m_PointMirror));
        }
    }

    AORenderPass::~AORenderPass()
    {
        SAFE_RELEASE(m_PointMirror);
        SAFE_RELEASE(m_NormalSRV);
        SAFE_RELEASE(m_NormalUAV);

        for (int i = 0; i < 2; i++) {
            SAFE_RELEASE(m_ViewSpaceDepthSRV[i]);
            SAFE_RELEASE(m_ViewSpaceDepthRTV[i]);
        }

        for (int i = 0; i < 2; i++) {
            SAFE_RELEASE(m_BlurIntermediateSRV[i]);
            SAFE_RELEASE(m_BlurIntermediateRTV[i]);
        }
    }

    void AORenderPass::pass(int pass) const
    {
        using namespace DirectX::SimpleMath;

        auto cam = Global::mainCamera;
        AOConstants constants = {};
        constants.DepthUnpack.x = (cam->getFarPlane() * cam->getNearPlane()) / (cam->getFarPlane() - cam->getNearPlane());
        constants.DepthUnpack.y = cam->getFarPlane() / (cam->getFarPlane() - cam->getNearPlane());

        auto proj = cam->getProj();

        constants.PerPassFullResCoordOffset[0] = pass % 2;
        constants.PerPassFullResCoordOffset[1] = pass / 2;

        constants.CameraHalfFOV = Vector2(1.f / proj.m[0][0], 1.f / proj.m[1][1]);
        constants.NDCToViewMul = constants.CameraHalfFOV * Vector2(2.f, -2.f);
        constants.NDCToViewAdd = constants.CameraHalfFOV * Vector2(-1.f, 1.f);

        constants.HalfViewport = Vector2(1.f / float(WIN_WIDTH / 2), 1.f / float(WIN_HEIGHT / 2));
        constants.Viewport = Vector2(1.f / float(WIN_WIDTH), 1.f / float(WIN_HEIGHT));
        constants.Viewport2X = constants.Viewport * 2.f;
        constants.Viewport2X025 = constants.Viewport2X * .25f;

        float offset = 0.f;
        float radius = 1.f;
        for (int i = 0; i < 5; i++) {
            int a = pass;
            int b = i;

            int map[5] = { 0, 1, 4, 3, 2 };
            b = map[i];

            float ca, sa;
            float angle = ((float)a + (float)b / (float)5) * DirectX::XM_PI * 0.5f;
            angle += offset;

            ca = cosf(angle);
            sa = sinf(angle);

            float scale = 1.f + (a - 1.5f + (b - (5 - 1.f) * .5f) / (float)5) * .07f;
            scale *= radius;

            constants.PatternRotScaleMatrices[i] = DirectX::SimpleMath::Vector4(scale * ca, scale * -sa, -scale * sa, -scale * ca);
        }

        m_ConstantsBuffer.write(Global::context, &constants, sizeof(constants));
    }

    void AORenderPass::render() const
    {

        auto cxt = Global::context;

        cxt->OMSetRenderTargets(0, nullptr, nullptr);

        auto point = Global::cStates->PointClamp();
        auto linear = Global::cStates->LinearClamp();

        cxt->PSSetSamplers(2, 1, &linear);

        D3D11_VIEWPORT viewport = {};
        viewport.Width = WIN_WIDTH / 2.f;
        viewport.Height = WIN_HEIGHT / 2.f;

        pass(0);

        // prepare viewspace depth & normals
        {
            cxt->IASetInputLayout(nullptr);
            cxt->VSSetShader(m_SSAOPrepareDepthNormals, nullptr, 0);
            cxt->PSSetShader(m_SSAOPrepareDepthNormals, nullptr, 0);

            cxt->PSSetSamplers(0, 1, &point);
            cxt->PSSetShaderResources(0, 1, &m_DepthSRV);
            cxt->PSSetConstantBuffers(0, 1, m_ConstantsBuffer);
            cxt->RSSetState(Global::cStates->CullNone());
            cxt->RSSetViewports(1, &viewport);
            cxt->OMSetRenderTargetsAndUnorderedAccessViews(2, m_ViewSpaceDepthRTV, nullptr, 2, 1, &m_NormalUAV, nullptr);

            cxt->Draw(3, 0);

            cxt->OMSetRenderTargetsAndUnorderedAccessViews(0, nullptr, nullptr, 2, 0, nullptr, nullptr);
        }


        cxt->PSSetSamplers(1, 1, &m_PointMirror);
        cxt->PSSetShaderResources(0, 1, &m_NormalSRV);
        
        // first half
        {
            cxt->PSSetShader(m_SSAOGenerate, nullptr, 0);
            cxt->PSSetShaderResources(1, 1, &m_ViewSpaceDepthSRV[0]);
            cxt->OMSetRenderTargets(1, &m_BlurIntermediateRTV[0], nullptr);

            // generate ao
            cxt->Draw(3, 0);

            cxt->PSSetShader(m_SSAOBlur, nullptr, 0);
            cxt->OMSetRenderTargets(1, &m_AOSlicesRTV[0], nullptr);
            cxt->PSSetShaderResources(1, 1, &m_BlurIntermediateSRV[0]);

            // blur
            cxt->Draw(3, 0);
        }

        pass(1);

        // second half
        {
            cxt->PSSetShader(m_SSAOGenerate, nullptr, 0);
            cxt->PSSetShaderResources(1, 1, &m_ViewSpaceDepthSRV[1]);
            cxt->OMSetRenderTargets(1, &m_BlurIntermediateRTV[1], nullptr);

            // generate ao
            cxt->Draw(3, 0);

            cxt->PSSetShader(m_SSAOBlur, nullptr, 0);
            cxt->OMSetRenderTargets(1, &m_AOSlicesRTV[1], nullptr);
            cxt->PSSetShaderResources(1, 1, &m_BlurIntermediateSRV[1]);

            // blur
            cxt->Draw(3, 0);
        };

        cxt->OMSetRenderTargets(0, nullptr, nullptr);
        viewport.Width = WIN_WIDTH;
        viewport.Height = WIN_HEIGHT;
        cxt->RSSetViewports(1, &viewport);

        cxt->PSSetShaderResources(0, 1, Global::nulls);

    }

    void AORenderPass::update(float deltaTime)
    {
    }

}
