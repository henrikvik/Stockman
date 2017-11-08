#include "GUIRenderPass.h"
#include "../RenderQueue.h"
#include "../Device.h"
#include "../Utility/sizeofv.h"
#include "../Utility/TextureLoader.h"
#include "../CommonState.h"
#include <comdef.h>

Graphics::GUIRenderPass::GUIRenderPass(std::initializer_list<ID3D11RenderTargetView*> targets, std::initializer_list<ID3D11ShaderResourceView*> resources, std::initializer_list<ID3D11Buffer*> buffers, ID3D11DepthStencilView * depthStencil)
    : RenderPass(targets, resources, buffers, depthStencil)
    , spriteShader(Resources::Shaders::SpriteShader)
    , vertexBuffer(CpuAccess::Write, INSTANCE_CAP(SpriteRenderInfo) * 4)
    , offsetBuffer(Global::device)
{
    this->sBatch = std::make_unique<DirectX::SpriteBatch>(Global::context);

    for (auto & font : Resources::Fonts::Paths)
    {
        fonts.insert(std::make_pair(
            font.first, 
            std::make_unique<DirectX::SpriteFont>(Global::device, _bstr_t(font.second))
        ));
    }

    createBlendState();
}

Graphics::GUIRenderPass::~GUIRenderPass()
{
    sBatch.reset();
    for (auto & font : fonts)
    {
        font.second.reset();
    }
    SAFE_RELEASE(blendState);
}

void Graphics::GUIRenderPass::render() const
{
    enum { TL, TR, BL, BR};
    Global::context->IASetInputLayout(nullptr);
    Global::context->PSSetShader(spriteShader, nullptr, 0);
    Global::context->VSSetShader(spriteShader, nullptr, 0);

    Global::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    Global::context->RSSetState(Global::cStates->CullNone());

    ID3D11SamplerState * linear = Global::cStates->LinearClamp();
    Global::context->PSSetSamplers(0, 1, &linear);

    Global::context->OMSetRenderTargets(targets.size(), targets.data(), depthStencil);

    
    static float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static UINT sampleMask = 0xffffffff;
    Global::context->OMSetBlendState(blendState, blendFactor, sampleMask);

    Global::context->VSSetShaderResources(0, 1, vertexBuffer);
    Global::context->VSSetConstantBuffers(0, 1, offsetBuffer);


    UINT offset = 0;
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
        Global::context->PSSetShaderResources(2, 1, *TextureLoader::get().getTexture(info->texture));
        offsetBuffer.write(Global::context, &offset, sizeof(offset));
        Global::context->Draw(4, offset);
        offset += 4;
    }

    textRender();

    Global::context->VSSetShaderResources(0, 1, Global::nulls);
    Global::context->VSSetConstantBuffers(0, 1, Global::nulls);
    Global::context->PSSetSamplers(0, 1, Global::nulls);
    Global::context->OMSetRenderTargets(targets.size(), Global::nulls, nullptr);
    Global::context->OMSetBlendState(Global::cStates->Opaque(), NULL, -1);
}

void Graphics::GUIRenderPass::update(float deltaTime)
{
    enum { TL, TR, BL, BR};

    std::vector<Vertex> vertices(4);

    size_t offset = 0;
    auto ptr = vertexBuffer.map(Global::context);
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
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

        *ptr++ = vertices[TL];
        *ptr++ = vertices[TR];
        *ptr++ = vertices[BL];
        *ptr++ = vertices[BR];
    }
    vertexBuffer.unmap(Global::context);
}

void Graphics::GUIRenderPass::createBlendState()
{
    D3D11_BLEND_DESC blendDesc = {0};

    blendDesc.IndependentBlendEnable = false;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    //blendDesc.RenderTarget[0].LogicOp = D3D11_LOGIC_OP_CLEAR;

    Global::device->CreateBlendState(&blendDesc, &blendState);

}

//render the queued text
void Graphics::GUIRenderPass::textRender() const
{
    sBatch->Begin(DirectX::SpriteSortMode_Deferred);
    for (auto & info : RenderQueue::get().getQueue<TextRenderInfo>())
    {
        if (isDrawableString(info->text))
        {
            fonts.at(info->font)->DrawString(sBatch.get(), info->text, info->position, info->color);
        }
        
    }
    sBatch->End();
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