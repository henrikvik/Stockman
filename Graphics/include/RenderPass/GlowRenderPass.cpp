#include "GlowRenderPass.h"
#include "../Device.h"
#include "../CommonState.h"
#include <Singletons\DebugWindow.h>

namespace Graphics
{
    GlowRenderPass::GlowRenderPass(
        ID3D11ShaderResourceView *bloomSRV,
        std::vector<ID3D11ShaderResourceView *> bloomSRVChain,
        std::vector<ID3D11RenderTargetView *> bloomRTVChain
    ) :
        RenderPass({}, {}, {}, nullptr),
        m_BloomSRV(bloomSRV),
        m_BloomSRVMipChain(bloomSRVChain),
        m_BloomRTVMipChain(bloomRTVChain),
        m_KawaseDualFilterDownsample(Resources::Shaders::KawaseDualFilterDownsample, ShaderType::PS),
        m_KawaseDualFilterUpsample(Resources::Shaders::KawaseDualFilterUpsample, ShaderType::PS),
        m_GaussianHorizontal(Resources::Shaders::GlowBlurHorizontal),
        m_GaussianVertical(Resources::Shaders::GlowBlurVertical)
    {
        createMips();

        DebugWindow::getInstance()->registerCommand("GFX_SET_GLOW", [&](std::vector<std::string> &args)->std::string
        {
            std::string catcher = "";
            try
            {
                if (args.size() != 0)
                {
                    enabled = std::stoi(args[0]);

                    if (enabled)
                        catcher = "Glow enabled!";

                    else
                        catcher = "Glow disabled!";
                }
                else
                {
                    catcher = "missing argument 0 or 1.";
                }
            }
            catch (const std::exception&)
            {
                catcher = "Argument must be 0 or 1.";
            }

            return catcher;
        });
    }

    GlowRenderPass::~GlowRenderPass()
    {
        SAFE_RELEASE(m_BlurTempSRV);
        SAFE_RELEASE(m_BlurTempRTV);
    }

    void GlowRenderPass::update(float deltaTime)
    {
    }

    void GlowRenderPass::render() const
    {
        if (!enabled)
            return;

        // PROFILE_BEGIN("Glow");
        
        Global::context->RSSetState(Global::cStates->CullNone());
        Global::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(m_GaussianHorizontal, nullptr, 0);
        Global::context->PSSetShader(m_KawaseDualFilterDownsample, nullptr, 0);
        
        auto sampler = Global::cStates->LinearClamp();
        Global::context->PSSetSamplers(0, 1, &sampler);

        //stupid first case
        Global::context->PSSetShaderResources(0, 1, &m_BloomSRVMipChain[0]);
        Global::context->OMSetRenderTargets(1, &m_BloomRTVMipChain[1], nullptr);

        setMipViewPort(1);

        Global::context->Draw(3, 0);

        Global::context->OMSetRenderTargets(1, Global::nulls, nullptr);
        Global::context->PSSetShaderResources(0, 1, Global::nulls);

        for (int i = 1; i < m_BloomRTVMipChain.size() - 1; i++)
        {
            Global::context->PSSetShaderResources(0, 1, &m_BloomSRVMipChain[i]);
            Global::context->OMSetRenderTargets(1, &m_BloomRTVMipChain[i + 1], nullptr);

            setMipViewPort(i + 1);

            Global::context->Draw(3, 0);

            Global::context->OMSetRenderTargets(1, Global::nulls, nullptr);
            Global::context->PSSetShaderResources(0, 1, Global::nulls);
        }

        Global::context->OMSetRenderTargets(0, nullptr, nullptr);
        Global::context->PSSetShaderResources(0, 1, Global::nulls);


        setMipViewPort(MIP_LEVELS - 1);

        Global::context->IASetInputLayout(nullptr);
        Global::context->VSSetShader(m_GaussianHorizontal, nullptr, 0);
        Global::context->PSSetShader(m_GaussianHorizontal, nullptr, 0);
        Global::context->OMSetRenderTargets(1, &m_BlurTempRTV, nullptr);
        Global::context->PSSetShaderResources(0, 1, &m_BloomSRVMipChain.back());
        Global::context->Draw(3, 0);

        Global::context->OMSetRenderTargets(1, Global::nulls, nullptr);
        Global::context->PSSetShaderResources(0, 1, Global::nulls);

        Global::context->OMSetRenderTargets(1, &m_BloomRTVMipChain.back(), nullptr);
        Global::context->PSSetShaderResources(0, 1, &m_BlurTempSRV);

        Global::context->VSSetShader(m_GaussianVertical, nullptr, 0);
        Global::context->PSSetShader(m_GaussianVertical, nullptr, 0);
        Global::context->Draw(3, 0);

        Global::context->OMSetRenderTargets(0, nullptr, nullptr);
        Global::context->PSSetShaderResources(0, 1, Global::nulls);

        setMipViewPort(0);

        Global::context->OMSetRenderTargets(1, Global::nulls, nullptr);
        Global::context->PSSetShaderResources(4, 1, Global::nulls);
        Global::context->PSSetShaderResources(5, 1, Global::nulls);

//         //         PROFILE_END();();
    }

    //Very ugly
    void GlowRenderPass::createMips()
    {
        ID3D11Texture2D * texture;

        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        textureDesc.Width = WIN_WIDTH;
        textureDesc.Height = WIN_HEIGHT;
        textureDesc.MipLevels = MIP_LEVELS;
        textureDesc.ArraySize = 1;
        textureDesc.SampleDesc.Count = 1;


        D3D11_RENDER_TARGET_VIEW_DESC renderDesc = {};
        renderDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
        resourceDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        resourceDesc.Texture2D.MipLevels = 1;


        auto p = (int)powf(2.f, (float)m_BloomRTVMipChain.size() - 1);
        textureDesc.Width = WIN_WIDTH / p;
        textureDesc.Height = WIN_HEIGHT / p;
        textureDesc.MipLevels = 1;
        ThrowIfFailed(Global::device->CreateTexture2D(&textureDesc, 0, &texture));

        auto glow_level = m_BloomRTVMipChain.size() - 1;
        resourceDesc.Texture2D.MipLevels = 1;
        resourceDesc.Texture2D.MostDetailedMip = 0;
        renderDesc.Texture2D.MipSlice = 0;

        ThrowIfFailed(Global::device->CreateRenderTargetView(texture, &renderDesc, &m_BlurTempRTV));
        ThrowIfFailed(Global::device->CreateShaderResourceView(texture, &resourceDesc, &m_BlurTempSRV));
        SAFE_RELEASE(texture);

    }
    
    void GlowRenderPass::setMipViewPort(int level) const
    {
        level = (int)powf(2.f, (float)level);
        D3D11_VIEWPORT viewPort = {};
        viewPort.Height = float(WIN_HEIGHT / level);
        viewPort.Width = float(WIN_WIDTH / level);
        viewPort.MaxDepth = 1.f;

        Global::context->RSSetViewports(1, &viewPort);
    }
}