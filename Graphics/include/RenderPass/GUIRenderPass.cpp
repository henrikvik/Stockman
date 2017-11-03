#include "GUIRenderPass.h"
#include "../RenderQueue.h"
#include "../Device.h"
#include "../Utility/sizeofv.h"
#include "../Utility/TextureLoader.h"

Graphics::GUIRenderPass::GUIRenderPass(ID3D11RenderTargetView * renderTarget)
    : spriteShader(Resources::Shaders::SpriteShader)
    , vertexBuffer(device, CpuAccess::Write, 4)
{
    this->renderTarget = renderTarget;
}

void Graphics::GUIRenderPass::render()
{
    enum { TL, TR, BL, BR};
    context->IASetInputLayout(nullptr);
    context->PSSetShader(spriteShader, nullptr, 0);
    context->VSSetShader(spriteShader, nullptr, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    context->RSSetState(cStates->CullNone());

    context->VSSetShaderResources(0, 1, vertexBuffer);

    ID3D11SamplerState * linear = cStates->LinearClamp();
    context->PSSetSamplers(0, 1, &linear);

    context->OMSetRenderTargets(1, &renderTarget, nullptr);

    std::vector<Vertex> vertices(4);
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
        context->PSSetShaderResources(2, 1, *TextureLoader::get().getTexture(info->texture));


        using namespace DirectX::SimpleMath;

    #define TL_X info->screenRect.topLeft.x
    #define TL_Y info->screenRect.topLeft.y
    #define BR_X info->screenRect.bottomRight.x
    #define BR_Y info->screenRect.bottomRight.y
        vertices[TL].position = Vector2(TL_X, TL_Y);
        vertices[TR].position = Vector2(BR_X, TL_Y);
        vertices[BL].position = Vector2(TL_X, BR_Y);
        vertices[BR].position = Vector2(BR_X, BR_Y);
    #undef TL_X
    #undef TL_Y
    #undef BR_X
    #undef BR_Y

    #define TL_X info->textureRect.topLeft.x
    #define TL_Y info->textureRect.topLeft.y
    #define BR_X info->textureRect.bottomRight.x
    #define BR_Y info->textureRect.bottomRight.y
        vertices[TL].uv = Vector2(TL_X, 1 - TL_Y);
        vertices[TR].uv = Vector2(BR_X, 1 - TL_Y);
        vertices[BL].uv = Vector2(TL_X, 1 - BR_Y);
        vertices[BR].uv = Vector2(BR_X, 1 - BR_Y);
    #undef TL_X
    #undef TL_Y
    #undef BR_X
    #undef BR_Y

        vertexBuffer.write(context, vertices.data(), sizeofv(vertices));
        context->Draw(4, 0);
    }
}
