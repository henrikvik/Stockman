#include "GUIRenderPass.h"
#include "../RenderQueue.h"
#include "../Device.h"
#include "../Utility/sizeofv.h"
#include "../Utility/TextureLoader.h"

Graphics::GUIRenderPass::GUIRenderPass(ID3D11RenderTargetView * renderTarget)
    : spriteShader(Resources::Shaders::SpriteShader)
    , vertexBuffer(Global::device, CpuAccess::Write, SpriteRenderInfo::INSTANCE_CAP * 4)
{
    this->renderTarget = renderTarget;
}

void Graphics::GUIRenderPass::render() const
{
    enum { TL, TR, BL, BR};
    Global::context->IASetInputLayout(nullptr);
    Global::context->PSSetShader(spriteShader, nullptr, 0);
    Global::context->VSSetShader(spriteShader, nullptr, 0);
    Global::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    Global::context->RSSetState(cStates->CullNone());

    Global::context->VSSetShaderResources(0, 1, vertexBuffer);

    ID3D11SamplerState * linear = cStates->LinearClamp();
    Global::context->PSSetSamplers(0, 1, &linear);

    Global::context->OMSetRenderTargets(1, &renderTarget, nullptr);
    Global::context->OMSetBlendState(cStates->AlphaBlend(), NULL, -1);

    std::vector<Vertex> vertices(4);
    size_t offset = 0;
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
        Global::context->PSSetShaderResources(2, 1, *TextureLoader::get().getTexture(info->texture));
        Global::context->Draw(4, offset);
        offset += 4;
    }

    for (auto & info : RenderQueue::get().getQueue<TextRenderInfo>())
    {
    }
}

void Graphics::GUIRenderPass::update(float deltaTime)
{
    enum { TL, TR, BL, BR};

    std::vector<Vertex> vertices(4);


    size_t offset = 0;
    auto ptr = vertexBuffer.map(Global::context);
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
        Global::context->PSSetShaderResources(2, 1, *TextureLoader::get().getTexture(info->texture));

        using namespace DirectX::SimpleMath;

        float TL_X = (info->screenRect.topLeft.x * 2 - 1);
        float TL_Y = (info->screenRect.topLeft.y * 2 - 1);
        float BR_X = (info->screenRect.bottomRight.x * 2 - 1);
        float BR_Y = (info->screenRect.bottomRight.y * 2 - 1);
        vertices[TL].position = Vector2(TL_X, TL_Y * -1);
        vertices[TR].position = Vector2(BR_X, TL_Y * -1);
        vertices[BL].position = Vector2(TL_X, BR_Y * -1);
        vertices[BR].position = Vector2(BR_X, BR_Y * -1);

        float TL_UV_X = info->textureRect.topLeft.x;
        float TL_UV_Y = info->textureRect.topLeft.y;
        float BR_UV_X = info->textureRect.bottomRight.x;
        float BR_UV_Y = info->textureRect.bottomRight.y;
        vertices[TL].uv = Vector2(TL_UV_X, TL_UV_Y);
        vertices[TR].uv = Vector2(BR_UV_X, TL_UV_Y);
        vertices[BL].uv = Vector2(TL_UV_X, BR_UV_Y);
        vertices[BR].uv = Vector2(BR_UV_X, BR_UV_Y);

        memcpy((char*)ptr + offset, vertices.data(), sizeofv(vertices));
        offset += sizeofv(vertices);
    }
    vertexBuffer.unmap(Global::context);
}
