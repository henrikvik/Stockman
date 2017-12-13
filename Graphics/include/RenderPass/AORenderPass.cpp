#include "AORenderPass.h"

#include <random>

namespace Graphics
{

#define KERNEL_SIZE 16

    AORenderPass::AORenderPass(
        ID3D11ShaderResourceView *depthSRV,
        ID3D11ShaderResourceView * (&viewSpaceDepthSRVSlices)[2],
        ID3D11RenderTargetView * (&viewSpaceDepthRTVSlices)[2]
    )
        : RenderPass({}, {}, {}, nullptr),
          m_SSAOPass(Resources::Shaders::SSAO),
          m_DepthSRV(depthSRV),
          m_ViewSpaceDepthSRV(viewSpaceDepthSRVSlices),
          m_ViewSpaceDepthRTV(viewSpaceDepthRTVSlices)
    {
        using namespace DirectX::SimpleMath;

        auto cam = Global::mainCamera;
        m_Constants.DepthUnpack.x = (cam->getFarPlane() * cam->getNearPlane()) / (cam->getFarPlane() - cam->getNearPlane());
        m_Constants.DepthUnpack.y = cam->getFarPlane() / (cam->getFarPlane() - cam->getNearPlane());

        auto proj = cam->getProj();

        m_Constants.CameraHalfFOV = Vector2(1.f / proj.m[0][0], 1.f / proj.m[1][1]);
        m_Constants.NDCToViewMul = m_Constants.CameraHalfFOV * Vector2(2.f, -2.f);
        m_Constants.NDCToViewAdd = m_Constants.CameraHalfFOV * Vector2(-1.f, 1.f);

        m_Constants.Viewport = Vector2(1.f / float(WIN_WIDTH), 1.f / float(WIN_HEIGHT));
        m_Constants.Viewport2X = m_Constants.Viewport * 2.f;

        m_ConstantsBuffer.write(Global::context, &m_Constants, sizeof(m_Constants));

        auto device = Global::device;

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Format = DXGI_FORMAT_R32G32_FLOAT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.Width = 4;
        desc.Height = 4;
        desc.SampleDesc.Count = 1;
        desc.MipLevels = 1;
        desc.ArraySize = 1;


        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        desc.Width = WIN_WIDTH;
        desc.Height = WIN_HEIGHT;

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

    AORenderPass::~AORenderPass()
    {
        SAFE_RELEASE(m_Random);
        SAFE_RELEASE(m_NormalSRV);
        SAFE_RELEASE(m_NormalUAV);
    }

    void AORenderPass::render() const
    {

        auto cxt = Global::context;

        ID3D11SamplerState *samplers[] =
        {
            Global::cStates->PointClamp(),
        };

        ID3D11ShaderResourceView *views[] = {
            m_DepthSRV,
        };

        D3D11_VIEWPORT viewport = {};
        viewport.Width = WIN_WIDTH / 2.f;
        viewport.Height = WIN_HEIGHT / 2.f;

        cxt->IASetInputLayout(nullptr);
        cxt->VSSetShader(m_SSAOPass, nullptr, 0);
        cxt->PSSetShader(m_SSAOPass, nullptr, 0);

        cxt->PSSetSamplers(0, 1, samplers);
        cxt->PSSetShaderResources(0, 1, views);
        cxt->PSSetConstantBuffers(0, 1, m_ConstantsBuffer);
        cxt->RSSetState(Global::cStates->CullNone());
        cxt->RSSetViewports(1, &viewport);

        cxt->OMSetRenderTargets(2, m_ViewSpaceDepthRTV, nullptr);

        cxt->Draw(3, 0);

        cxt->OMSetRenderTargets(0, nullptr, nullptr);
        viewport.Width = WIN_WIDTH;
        viewport.Height = WIN_HEIGHT;
        cxt->RSSetViewports(1, &viewport);
    }

    void AORenderPass::update(float deltaTime)
    {
        m_Constants.Projection = Global::mainCamera->getProj();
        m_Constants.View = Global::mainCamera->getView();
        m_ConstantsBuffer.write(Global::context, &m_Constants, sizeof(m_Constants));
    }

}
