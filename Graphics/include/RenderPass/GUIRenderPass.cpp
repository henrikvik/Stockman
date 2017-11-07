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

    this->sBatch = std::make_unique<DirectX::SpriteBatch>(context);

    this->sFonts[0] = std::make_unique<DirectX::SpriteFont>(device, convertFontFilePath(Resources::Fonts::KG14).c_str());
    this->sFonts[1] = std::make_unique<DirectX::SpriteFont>(device, convertFontFilePath(Resources::Fonts::KG18).c_str());
    this->sFonts[2] = std::make_unique<DirectX::SpriteFont>(device, convertFontFilePath(Resources::Fonts::KG26).c_str());
}

Graphics::GUIRenderPass::~GUIRenderPass()
{
    sBatch.reset();
    for (size_t i = 0; i < NR_OF_FONTS; i++)
    {
        sFonts[i].reset();
    }
    
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
    context->OMSetBlendState(cStates->AlphaBlend(), NULL, -1);

    std::vector<Vertex> vertices(4);
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
        context->PSSetShaderResources(2, 1, *TextureLoader::get().getTexture(info->texture));

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

        vertexBuffer.write(context, vertices.data(), sizeofv(vertices));
        context->Draw(4, 0);
    }

    textRender();
}

//render the queued text
void Graphics::GUIRenderPass::textRender()
{
    sBatch->Begin(DirectX::SpriteSortMode_Deferred);
    for (auto & info : RenderQueue::get().getQueue<TextRenderInfo>())
    {
        if (isDrawableString(info->text))
        {
            sFonts[info->font]->DrawString(sBatch.get(), info->text, info->position, info->color);
        }
        
    }
    sBatch->End();
}

std::wstring Graphics::GUIRenderPass::convertFontFilePath(Resources::Fonts::Files input)
{

    const char* orig = Resources::Fonts::Paths.at(input);
    size_t size = std::strlen(orig);
    std::wstring converted;
    converted.resize(size);
    std::mbstowcs(&converted[0], orig, size);
    return converted;
}

//checks fow unallowed characters that chrashed the text draw
bool Graphics::GUIRenderPass::isDrawableString(const wchar_t * text) const
{
    for (size_t i = 0; i < std::wcslen(text); i++)
    {
        if (text[i] > 127 || text[i] < 32)
        {
            return false;
        }
    }
    return true;
}
