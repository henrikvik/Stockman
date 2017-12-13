#pragma once
#include "RenderPass.h"
#include <SpriteFont.h>

namespace Graphics
{
    class FancyDepthRenderPass : public RenderPass
    {
    public:
        FancyDepthRenderPass(ID3D11DepthStencilView *depthStencil);
        virtual ~FancyDepthRenderPass() {};

        virtual wchar_t* name() const override {
            return L"ParticleDepthRenderPass";
        }

        virtual void render() const override;
        virtual void update(float deltaTime) override;
    };

    class FancyRenderPass : public RenderPass
    {
    public:
        FancyRenderPass(
            ID3D11RenderTargetView *output,
            ID3D11ShaderResourceView *lightGrid,
            ID3D11ShaderResourceView *lightIndexList,
            ID3D11ShaderResourceView *lights,
            ID3D11ShaderResourceView *shadowMap,
            ID3D11Buffer *lightBuffer,
            ID3D11DepthStencilView *depthStencil);
        virtual ~FancyRenderPass() {};

        virtual wchar_t* name() const override {
            return L"FancyRenderPass";
        }

        virtual void render() const override;
        virtual void update(float deltaTime) override;
    private:
        ID3D11ShaderResourceView *m_FontTexture;
        DirectX::SpriteFont *m_Font;

        ID3D11RenderTargetView *m_Output;

        ID3D11ShaderResourceView *m_LightGrid;
        ID3D11ShaderResourceView *m_LightIndexList;
        ID3D11ShaderResourceView *m_Lights;
        ID3D11ShaderResourceView *m_ShadowMap;
        ID3D11Buffer *m_LightBuffer;
    };
}
