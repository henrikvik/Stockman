#pragma once
#include "RenderPass.h"
#include "../Utility/ShaderResource.h"
#include "../Utility/StructuredBuffer.h"
#include "../Utility/Shader.h"
#include <CommonStates.h>
#include <SimpleMath.h>
#include <SpriteFont.h>
#include <SpriteBatch.h>

#define NR_OF_FONTS 3

namespace Graphics
{
    class GUIRenderPass : public RenderPass
    {
    public:
        GUIRenderPass(
            std::initializer_list<ID3D11RenderTargetView*> targets,
            std::initializer_list<ID3D11ShaderResourceView*> resources = {},
            std::initializer_list<ID3D11Buffer*> buffers = {},
            ID3D11DepthStencilView * depthStencil = nullptr
        );
        virtual ~GUIRenderPass();

        // Inherited via RenderPass
        virtual void render() const override;
        virtual void update(float deltaTime) override;
    private:
        Shader spriteShader;

        struct Vertex
        {
            DirectX::SimpleMath::Vector2 position;
            DirectX::SimpleMath::Vector2 uv;
        };
        StructuredBuffer<Vertex> vertexBuffer;

        std::unique_ptr<DirectX::SpriteBatch> sBatch;
        std::unordered_map<Resources::Fonts::Files, std::unique_ptr<DirectX::SpriteFont>> fonts;
        ConstantBuffer<UINT> offsetBuffer;

        ID3D11BlendState* blendState;
        void createBlendState();

        void textRender() const;
        bool isDrawableString(const wchar_t * text)const;
    };
}