#include "GUIRenderPass.h"
#include "../RenderQueue.h"
#include "../Device.h"
#include "../Utility/sizeofv.h"
#include "../Utility/TextureLoader.h"
#include "../MainCamera.h"
#include "../CommonState.h"
#include <comdef.h>
#define SHAKE_DIRECTION_MULTIPLIER 2.f

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

}

Graphics::GUIRenderPass::~GUIRenderPass()
{
    sBatch.reset();
    for (auto & font : fonts)
    {
        font.second.reset();
    }
}

void Graphics::GUIRenderPass::render() const
{
    PROFILE_BEGIN("GUI");
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
    Global::context->OMSetBlendState(Global::transparencyBlendState, blendFactor, sampleMask);

    Global::context->VSSetShaderResources(0, 1, vertexBuffer);
    Global::context->VSSetConstantBuffers(0, 1, offsetBuffer);


    UINT offset = 0;
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
        Global::context->PSSetShaderResources(2, 1, *TextureLoader::get().getTexture(info.texture));
        offsetBuffer.write(Global::context, &offset, sizeof(offset));
        Global::context->Draw(4, 0);
        offset += 4;
    }

    textRender();

    Global::context->VSSetShaderResources(0, 1, Global::nulls);
    Global::context->PSSetShaderResources(2, 1, Global::nulls);
    Global::context->VSSetConstantBuffers(0, 1, Global::nulls);
    Global::context->PSSetSamplers(0, 1, Global::nulls);
    Global::context->OMSetRenderTargets(targets.size(), Global::nulls, nullptr);
    Global::context->OMSetBlendState(Global::cStates->Opaque(), NULL, -1);
    PROFILE_END();
}

void Graphics::GUIRenderPass::update(float deltaTime)
{
    updateShake(deltaTime);
    enum { TL, TR, BL, BR};

    std::vector<Vertex> vertices(4);

    size_t offset = 0;
    auto ptr = vertexBuffer.map(Global::context);
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
        using namespace DirectX::SimpleMath;

        float TL_X = (info.screenRect.topLeft.x * 2 - 1);
        float TL_Y = (info.screenRect.topLeft.y * 2 - 1);
        float BR_X = (info.screenRect.bottomRight.x * 2 - 1);
        float BR_Y = (info.screenRect.bottomRight.y * 2 - 1);
        vertices[TL].position = Vector2(TL_X, TL_Y * -1) + ndcPositionOffset;
        vertices[TR].position = Vector2(BR_X, TL_Y * -1) + ndcPositionOffset;
        vertices[BL].position = Vector2(TL_X, BR_Y * -1) + ndcPositionOffset;
        vertices[BR].position = Vector2(BR_X, BR_Y * -1) + ndcPositionOffset;

        float TL_UV_X = info.textureRect.topLeft.x;
        float TL_UV_Y = info.textureRect.topLeft.y;
        float BR_UV_X = info.textureRect.bottomRight.x;
        float BR_UV_Y = info.textureRect.bottomRight.y;
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

void Graphics::GUIRenderPass::updateShake(float deltaTime)
{
    using namespace DirectX::SimpleMath;

    static float shakeCounter = 0;
    static float shakeDuration = 0;
    static float shakeRadius = 0;
    static bool isShaking = 0;
    static Vector2 shakeDir;

    for (auto & info : RenderQueue::get().getQueue<SpecialEffectRenderInfo>())
    {
        if (info.type == info.screenShake)
        {
            shakeCounter = 0;
            shakeDuration = info.duration;
            shakeRadius = info.radius;
            isShaking = true;

            shakeDir = info.direction;
            shakeDir.Normalize();
        }
    }

    if (isShaking)
    {
        float currentShakeAmount = shakeRadius * (1 - (shakeCounter / shakeDuration));

        static float angle = 0;
        angle = rand() % 360;
        positionOffset = (Vector2(sin(angle), cos(angle)) + (shakeDir * SHAKE_DIRECTION_MULTIPLIER)) * currentShakeAmount;



        shakeCounter += deltaTime;
        if (shakeCounter >= shakeDuration)
        {
            isShaking = false;
            positionOffset = Vector2(0, 0);
        }

        ndcPositionOffset = Vector2((float)(positionOffset.x * 2.f / WIN_WIDTH), ((WIN_HEIGHT - positionOffset.y) / WIN_HEIGHT) - 1.f);

        //this ugly
        Global::mainCamera->update(Global::mainCamera->getPos() + ndcPositionOffset * 3.f, Global::mainCamera->getForward(), Global::context);
    }
}

//render the queued text
void Graphics::GUIRenderPass::textRender() const
{
    sBatch->Begin(DirectX::SpriteSortMode_Deferred);
    for (auto & info : RenderQueue::get().getQueue<TextRenderInfo>())
    {
        if (isDrawableString(info.text))
        {
            fonts.at(info.font)->DrawString(sBatch.get(), info.text, info.position + positionOffset, info.color);
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
