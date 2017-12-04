#include "AORenderPass.h"

#include <random>

namespace Graphics
{

#define KERNEL_SIZE 16

    AORenderPass::AORenderPass(
        ID3D11RenderTargetView *output,
        ID3D11ShaderResourceView *position,
        ID3D11ShaderResourceView *normal
    )
        : RenderPass({}, {}, {}, nullptr),
          m_SSAOPass(Resources::Shaders::SSAO),
          m_Output(output),
          m_Position(position),
          m_Normal(normal)
    {
        using namespace DirectX::SimpleMath;

        std::uniform_real_distribution<float> random(0.0, 1.0); // random floats between 0.0 - 1.0
        std::default_random_engine generator;
        std::vector<Vector4> kernel;
        for (unsigned int i = 0; i < KERNEL_SIZE; ++i)
        {
            Vector3 sample = {
                random(generator) * 2.0f - 1.0f,
                random(generator) * 2.0f - 1.0f,
                random(generator),
            };
            sample.Normalize();
            sample *= random(generator);
            float scale = (float)i / KERNEL_SIZE;
            float a = 0.1f;
            float b = 1.f;
            float f = scale * scale;
            scale = a + f * (b - a);
            sample *= scale;
            m_Constants.Kernel[i] = Vector4(sample);
        }

        m_ConstantsBuffer.write(Global::context, &m_Constants, sizeof(m_Constants));

        std::vector<Vector2> noise;
        for (unsigned int i = 0; i < 16; i++)
        {
            Vector2 dir = {
                random(generator) * 2.0f - 1.0f,
                random(generator) * 2.0f - 1.0f,
            };
            noise.push_back(dir);
        }

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Format = DXGI_FORMAT_R32G32_FLOAT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.Width = 4;
        desc.Height = 4;
        desc.SampleDesc.Count = 1;
        desc.MipLevels = 1;
        desc.ArraySize = 1;

        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = noise.data();
        data.SysMemPitch = 4 * 2 * 2;

        ID3D11Texture2D *texture = nullptr;
        ThrowIfFailed(Global::device->CreateTexture2D(&desc, &data, &texture));
        ThrowIfFailed(Global::device->CreateShaderResourceView(texture, nullptr, &m_Random));
        SAFE_RELEASE(texture);
    }

    AORenderPass::~AORenderPass()
    {
        SAFE_RELEASE(m_Random);
    }

    void AORenderPass::render() const
    {

        auto cxt = Global::context;

        ID3D11SamplerState *samplers[] =
        {
            Global::cStates->PointWrap(),
            //Global::cStates->PointWrap()
        };

        ID3D11ShaderResourceView *views[] = {
            m_Position,
            m_Normal,
            m_Random
        };

        cxt->IASetInputLayout(nullptr);
        cxt->VSSetShader(m_SSAOPass, nullptr, 0);
        cxt->PSSetShader(m_SSAOPass, nullptr, 0);
        cxt->PSSetSamplers(0, 1, samplers);
        cxt->PSSetShaderResources(0, 3, views);
        cxt->PSSetConstantBuffers(0, 1, m_ConstantsBuffer);
        cxt->RSSetState(Global::cStates->CullNone());

        cxt->OMSetRenderTargets(1, &m_Output, nullptr);

        cxt->Draw(3, 0);

        cxt->OMSetRenderTargets(0, nullptr, nullptr);
    }

    void AORenderPass::update(float deltaTime)
    {
        m_Constants.Projection = Global::mainCamera->getProj();
        m_Constants.View = Global::mainCamera->getView();
        m_ConstantsBuffer.write(Global::context, &m_Constants, sizeof(m_Constants));
    }

}
