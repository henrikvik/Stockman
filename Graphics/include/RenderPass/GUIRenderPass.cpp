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
    , alphabuffer(CpuAccess::Write, sizeof(float) * INSTANCE_CAP(float))
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
    affectEverything = false;
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
    Global::context->PSSetShaderResources(1, 1, alphabuffer);


    PROFILE_BEGIN("Sprite");
    UINT offset = 0;
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
        Global::context->PSSetShaderResources(2, 1, *TextureLoader::get().getTexture(info.texture));
        offsetBuffer.write(Global::context, &offset, sizeof(offset));
        Global::context->Draw(4, 0);
        offset += 4;
    }
    PROFILE_END();

    PROFILE_BEGIN("Text");
    textRender();
    PROFILE_END();

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
    updateBounce(deltaTime);
    enum { TL, TR, BL, BR};

    std::vector<Vertex> vertices(4);

    size_t offset = 0;
    auto ptr = vertexBuffer.map(Global::context);
    auto alphaptr = alphabuffer.map(Global::context);
    for (auto & info : RenderQueue::get().getQueue<SpriteRenderInfo>())
    {
        using namespace DirectX::SimpleMath;

        float TL_X = (info.screenRect.topLeft.x * 2 - 1);
        float TL_Y = (info.screenRect.topLeft.y * 2 - 1);
        float BR_X = (info.screenRect.bottomRight.x * 2 - 1);
        float BR_Y = (info.screenRect.bottomRight.y * 2 - 1);

        if (info.isMoveable || affectEverything)
        {
            vertices[TL].position = Vector2(TL_X, TL_Y * -1) + ndcPositionOffset;
            vertices[TR].position = Vector2(BR_X, TL_Y * -1) + ndcPositionOffset;
            vertices[BL].position = Vector2(TL_X, BR_Y * -1) + ndcPositionOffset;
            vertices[BR].position = Vector2(BR_X, BR_Y * -1) + ndcPositionOffset;
        }
        else
        {
            vertices[TL].position = Vector2(TL_X, TL_Y * -1);
            vertices[TR].position = Vector2(BR_X, TL_Y * -1);
            vertices[BL].position = Vector2(TL_X, BR_Y * -1);
            vertices[BR].position = Vector2(BR_X, BR_Y * -1);
        }
       

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
        *alphaptr++ = info.alpha;
    }
    vertexBuffer.unmap(Global::context);
    alphabuffer.unmap(Global::context);
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

            affectEverything = info.affectEveryThing;
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

       
        if (affectEverything)
        {
            Vector3 forward = Global::mainCamera->getForward();
            //this very ugly
            Global::mainCamera->update(Global::mainCamera->getPos() + ndcPositionOffset * 3.f, forward, Global::context);
        }
        
    }
}

void Graphics::GUIRenderPass::updateBounce(float deltaTime)
{
    static float bDuration = 0.0f;
    static float bTime = 0.0f;
    static float bHeight = 0.0f;
    static float bMax = 0.0f;
    static DirectX::SimpleMath::Vector2 direction(0.0f, 0.0f);
    static bool isBouncing = false;
    static bool complete = false;

    for (auto & info : RenderQueue::get().getQueue<SpecialEffectRenderInfo>())
    {
        if (info.type == SpecialEffectRenderInfo::screenBounce)
        {
            bTime = 0.0f;
            bDuration = info.duration;
            bHeight = info.radius;
            direction = info.direction;
            direction.Normalize();
            isBouncing = true;
            bMax = info.bounceMax * direction.y;
            affectEverything = info.affectEveryThing;
        }
    }

    if (isBouncing)
    {
        bTime += deltaTime;
        if (bTime < bDuration)
        {
            /*positionOffset = DirectX::SimpleMath::Vector2(direction.x * bHeight * bTime, direction.y * bHeight * bTime);
            ndcPositionOffset = DirectX::SimpleMath::Vector2((float)(positionOffset.x * 2.f / WIN_WIDTH), ((WIN_HEIGHT - positionOffset.y) / WIN_HEIGHT) - 1.f);*/

            if (positionOffset.y < bMax && complete == false)
            {
                positionOffset.y += direction.y * bHeight * deltaTime;
            }
            else if (complete)
            {
                positionOffset.y -= direction.y * bHeight * deltaTime;
            }
            else
            {
                complete = true;
            }

            
            ndcPositionOffset = DirectX::SimpleMath::Vector2((float)(positionOffset.x * 2.f / WIN_WIDTH), ((WIN_HEIGHT - positionOffset.y) / WIN_HEIGHT) - 1.f);
        }
        else
        {
            isBouncing = false;
            complete = false;
            positionOffset = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
            ndcPositionOffset = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
        }
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
            if (info.isMoveable || affectEverything)
            {
                fonts.at(info.font)->DrawString(sBatch.get(), info.text.c_str(), info.position + positionOffset, info.color);
            }
            else 
            {
                fonts.at(info.font)->DrawString(sBatch.get(), info.text.c_str(), info.position, info.color);
            }
            
        }
        
    }
    sBatch->End();
}

//checks fow unallowed characters that chrashed the text draw
bool Graphics::GUIRenderPass::isDrawableString(std::wstring text) const
{
    for (size_t i = 0; i < text.length(); i++)
    {
        if (text[i] > 127 || text[i] < 32)
        {
            return false;
        }
    }
    return true;
}
