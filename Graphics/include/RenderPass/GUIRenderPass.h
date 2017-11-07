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

        // Inherited via RenderPass
        virtual void render() const override;
        virtual void update(float deltaTime) override;
    private:
        Shader spriteShader;

        ID3D11RenderTargetView * renderTarget;
        ID3D11BlendState * blendState;

        struct Vertex
        {
            DirectX::SimpleMath::Vector2 position;
            DirectX::SimpleMath::Vector2 uv;
        };

        StructuredBuffer<Vertex> vertexBuffer;

        std::unique_ptr<DirectX::SpriteBatch> sBatch;
        std::unique_ptr<DirectX::SpriteFont> sFonts[NR_OF_FONTS];
        void textRender();
        std::wstring convertFontFilePath(Resources::Fonts::Files input);
        bool isDrawableString(const wchar_t * text)const;
    };
}