#pragma once
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include <Resources\Resources.h>

struct RenderInfo
{
    static constexpr size_t INSTANCE_CAP = 3000;
};

struct DebugRenderInfo : RenderInfo
{
    std::vector<DirectX::SimpleMath::Vector3> * points;
    D3D11_PRIMITIVE_TOPOLOGY topology;
    DirectX::SimpleMath::Color color;
    bool useDepth;
};

struct LightRenderInfo : RenderInfo
{
    DirectX::SimpleMath::Vector3 position;
    DirectX::SimpleMath::Color color;
    float intensity;
    float range;
};


struct SpecialEffectRenderInfo : RenderInfo
{
    enum SpecialEffect { BulletTime };
    SpecialEffect effect;
    float progress;
};


struct SpriteRenderInfo : RenderInfo
{
    Resources::Textures::Files texture;
    DirectX::SimpleMath::Rectangle textureRect;
    DirectX::SimpleMath::Rectangle screenRect;
};

struct TextRenderInfo : RenderInfo
{
    Resources::Fonts::Files font;
    const wchar_t * text;
    DirectX::SimpleMath::Vector2 position;
    DirectX::SimpleMath::Color color;
};

struct StaticRenderInfo : RenderInfo
{
    Resources::Models::Files model;
    DirectX::SimpleMath::Matrix transform;
};

struct FoliageRenderInfo : StaticRenderInfo
{
};

struct AnimatedRenderInfo : StaticRenderInfo
{
    const char * animationName;
    float animationProgress;
};

struct EnemyRenderInfo : AnimatedRenderInfo
{
    float freeze;
    float burn;
};

struct PlayerRenderInfo : AnimatedRenderInfo
{
};
