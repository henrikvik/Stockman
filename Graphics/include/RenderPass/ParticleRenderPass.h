#pragma once
#include "RenderPass.h"

namespace Graphics
{
    class ParticleDepthRenderPass : public RenderPass
    {
    public:
        ParticleDepthRenderPass(ID3D11DepthStencilView *depthStencil);
        virtual ~ParticleDepthRenderPass() {};

        virtual wchar_t* name() const override {
            return L"ParticleDepthRenderPass";
        }

        virtual void render() const override;
        virtual void update(float deltaTime) override;
    };

    class ParticleRenderPass : public RenderPass
    {
    public:
        ParticleRenderPass(
            ID3D11RenderTargetView *output,
            ID3D11ShaderResourceView *lightGrid,
            ID3D11ShaderResourceView *lightIndexList,
            ID3D11ShaderResourceView *lights,
            ID3D11ShaderResourceView *shadowMap,
            ID3D11Buffer *lightBuffer,
            ID3D11DepthStencilView *depthStencil);
        virtual ~ParticleRenderPass() {};

        virtual wchar_t* name() const override {
            return L"ParticleRenderPass";
        }

        virtual void render() const override;
        virtual void update(float deltaTime) override;
    private:
        ID3D11RenderTargetView *m_Output;

        ID3D11ShaderResourceView *m_LightGrid;
        ID3D11ShaderResourceView *m_LightIndexList;
        ID3D11ShaderResourceView *m_Lights;
        ID3D11ShaderResourceView *m_ShadowMap;
        ID3D11Buffer *m_LightBuffer;
    };
}
