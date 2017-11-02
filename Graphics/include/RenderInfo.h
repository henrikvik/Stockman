#pragma once
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include <Resources\Resources.h>

struct FloatRect
{
    FloatRect()
        : topLeft(0,0)
        , bottomRight(1,1)
    {};

    FloatRect(DirectX::SimpleMath::Vector2 topLeft, DirectX::SimpleMath::Vector2 bottomRight)
        : topLeft(topLeft)
        , bottomRight(bottomRight)
    {};

    FloatRect(float x, float y, float width, float height)
        : topLeft(x, y)
        , bottomRight(x + width, y + height)
    {};

    DirectX::SimpleMath::Vector2 topLeft;
    DirectX::SimpleMath::Vector2 bottomRight;
};

struct RenderInfo
{
    static constexpr size_t INSTANCE_CAP = 3000;
};

struct DebugRenderInfo : RenderInfo
{
    std::vector<DirectX::SimpleMath::Vector3> * points;
    D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
    DirectX::SimpleMath::Color color;
    bool useDepth = false;
};

struct LightRenderInfo : RenderInfo
{
    DirectX::SimpleMath::Vector3 position;
    DirectX::SimpleMath::Color color;
    float intensity = 1;
    float range = 5;
};


struct SpecialEffectRenderInfo : RenderInfo
{
    enum SpecialEffect { BulletTime };
    SpecialEffect effect;
    float progress = 0;
};


struct SpriteRenderInfo : RenderInfo
{
    Resources::Textures::Files texture;
    FloatRect screenRect;
    FloatRect textureRect;
    float alpha = 1;
};

struct TextRenderInfo : RenderInfo
{
    Resources::Fonts::Files font;
    const wchar_t * text = L"";
    DirectX::SimpleMath::Vector2 position;
    DirectX::SimpleMath::Color color;
};

struct StaticRenderInfo : RenderInfo
{
    Resources::Models::Files model = Resources::Models::UnitCube;
    DirectX::SimpleMath::Matrix transform;
};

struct FoliageRenderInfo : StaticRenderInfo
{
};

struct AnimatedRenderInfo : StaticRenderInfo
{
    const char * animationName = "";
    float animationProgress = 0;
};

struct EnemyRenderInfo : AnimatedRenderInfo
{
    float freeze = 0;
    float burn = 0;
};

struct PlayerRenderInfo : AnimatedRenderInfo
{
};
