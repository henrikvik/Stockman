#include "SkyBoxRenderPass.h"
#include "../Device.h"
#include "../MainCamera.h"
#include "../CommonState.h"
#include <WICTextureLoader.h>
#include <Singeltons\DebugWindow.h>

namespace Graphics
{
    static bool DebugSkyBox = false;

    SkyBoxRenderPass::SkyBoxRenderPass(
        std::initializer_list<ID3D11RenderTargetView*> targets,
        std::initializer_list<ID3D11ShaderResourceView*> resources,
        std::initializer_list<ID3D11Buffer*> buffers,
        ID3D11DepthStencilView * depthStencil,
        Sun *sun) :
        RenderPass(targets, resources, buffers, depthStencil),
        skyShader(Resources::Shaders::SkyShader, ShaderType::VS | ShaderType::PS, HybrisLoader::Vertex::INPUT_DESC),
        m_MoonShader(Resources::Shaders::Moon),
        sphereTransformBuffer(Global::device),
        m_Colors({
            DirectX::SimpleMath::Vector4 { 75.0f/256.0f, 97.0f/256.0f, 199.0f/250.0f, 0.f },
            DirectX::SimpleMath::Vector4{ 13.0f / 256.0f, 17.0f / 256.0f, 79.0f / 250.0f, 0.f} ,
            DirectX::SimpleMath::Vector4(0.f, 0.f, 0.f, 0.25f)
        }),
        m_SkyColors(Global::device),
        m_Sun(sun)
    {
        this->skySphere = &HybrisLoader::HybrisLoader::get().getModel(Resources::Models::SkySphere)->getMesh();
        DirectX::CreateWICTextureFromFile(Global::device, L"../Resources/Textures/Diffusemoon.dds", nullptr, &m_MoonTexture);
        DebugWindow::getInstance()->registerCommand("GFX_DEBUG_SKYBOX", [&](std::vector<std::string> &args)->std::string
        {
            DebugSkyBox = !DebugSkyBox;

            std::string response = "DebugSkyBox = ";
            response += DebugSkyBox ? "true" : "false";

            return response;
        });
    }

    SkyBoxRenderPass::~SkyBoxRenderPass() {
        SAFE_RELEASE(m_MoonTexture)
    }

    void SkyBoxRenderPass::render() const
    {
        auto cxt = Global::context;

        PROFILE_BEGIN("Sky Box");
        cxt->RSSetState(Global::cStates->CullCounterClockwise());

        cxt->IASetInputLayout(skyShader);
        cxt->VSSetShader(skyShader, nullptr, 0);
        cxt->PSSetShader(skyShader, nullptr, 0);


        cxt->VSSetConstantBuffers(0, 1, *Global::mainCamera->getBuffer());
        cxt->VSSetConstantBuffers(4, 1, sphereTransformBuffer);

        cxt->PSSetConstantBuffers(1, 1, &buffers[0]);
        cxt->PSSetConstantBuffers(5, 1, m_SkyColors);
        cxt->OMSetRenderTargets(1, &targets[0], depthStencil);

        UINT offsets[] = {0};
        UINT strides[] = {HybrisLoader::Vertex::STRIDE};
        cxt->IASetVertexBuffers(0, 1, skySphere->getVertexBuffer(), strides, offsets);
        cxt->Draw(skySphere->getVertexCount(), 0);

    

        cxt->VSSetShader(m_MoonShader, nullptr, 0);
        cxt->VSSetConstantBuffers(1, 1, m_SkyColors);

        cxt->PSSetShader(m_MoonShader, nullptr, 0);

        auto sampler = Global::cStates->LinearClamp();
        cxt->PSSetShaderResources(0, 1, &m_MoonTexture);
        cxt->PSSetSamplers(0, 1, &sampler);

        float BLEND[4] = { 0.f, 0.f, 0.f, 1.f };
        cxt->OMSetBlendState(Global::cStates->AlphaBlend(), BLEND, 0xffffffff);

        cxt->RSSetState(Global::cStates->CullNone());
        cxt->OMSetDepthStencilState(Global::cStates->DepthRead(), 0x0);
        cxt->Draw(3, 0);

        cxt->OMSetBlendState(Global::cStates->Opaque(), BLEND, 0xffffffff);
        cxt->RSSetState(Global::cStates->CullClockwise());
        cxt->OMSetRenderTargets(1, Global::nulls, nullptr);
        PROFILE_END();
    }

    void SkyBoxRenderPass::update(float deltaTime)
    {
        using namespace DirectX::SimpleMath;

        if (DebugSkyBox) {
            ImGui::Begin("Skybox");
            ImGui::ColorEdit3("Base", (float*)&m_Colors.base);
            ImGui::ColorEdit3("Top", (float*)&m_Colors.top);
            ImGui::TextDisabled("Moon");
            ImGui::DragFloat("Size", &m_Colors.angleSize.w, 0.01f);
            ImGui::End();
        }

        auto forward = Global::mainCamera->getForward();
        auto pos = m_Sun->globalLight.position;
        auto dir = m_Sun->m_LightDir;
        dir.Normalize();

        m_Colors.angleSize.x = dir.x;
        m_Colors.angleSize.y = dir.y;
        m_Colors.angleSize.z = dir.z;

        auto view = Matrix::CreateBillboard(Vector3(dir), Vector3(0.f), Vector3::UnitY) * Matrix::CreateTranslation(Global::mainCamera->getPos());

        m_Colors.view = view;
        m_SkyColors.write(Global::context, &m_Colors, sizeof(m_Colors));

        DirectX::SimpleMath::Matrix temp = DirectX::SimpleMath::Matrix::CreateTranslation(Global::mainCamera->getPos());

        sphereTransformBuffer.write(Global::context, &temp, sizeof(temp));
    }
}
