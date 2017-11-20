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

    bool contains(float x, float y)
    {
        return (x >= topLeft.x) && (x < bottomRight.x) && (y >= topLeft.y) && (y < bottomRight.y);
    }
};

template<typename T>
struct InstanceCap { static constexpr size_t value = 1000; };
#define SET_INSTANCE_CAP(info, cap) template<> struct InstanceCap<info> { static constexpr size_t value = cap; };
#define INSTANCE_CAP(info) InstanceCap<info>::value

struct DebugBlame
{
    const char * FILE = "";
    size_t LINE = 0;
};

struct RenderInfo
#if _DEBUG
    : DebugBlame
#endif
{
};


struct DebugRenderInfo : RenderInfo
{
    std::vector<DirectX::SimpleMath::Vector3> * points;
    D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
    DirectX::SimpleMath::Color color;
    bool useDepth = false;
};
SET_INSTANCE_CAP(DebugRenderInfo, 10000)

struct NewDebugRenderInfo : RenderInfo
{
    struct Point
    {
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Color color;
    };

    D3D11_PRIMITIVE_TOPOLOGY topology;
    DirectX::SimpleMath::Color color;
    std::vector<Point> * points;
    bool useDepth;
};
SET_INSTANCE_CAP(NewDebugRenderInfo, 10000)

struct LightRenderInfo : RenderInfo
{

    DirectX::SimpleMath::Vector3 position;
    DirectX::SimpleMath::Color color;
    float intensity = 1;
    float range = 5;
};

SET_INSTANCE_CAP(LightRenderInfo, 128)

struct SpecialEffectRenderInfo : RenderInfo
{
    enum SpecialEffect { BulletTime, Snow, screenShake };
    SpecialEffect type;
    DirectX::SimpleMath::Vector2 direction;
    float progress = 0;
    float duration = 0;
    float radius = 0;
    bool restart = 0;
};

struct SpriteRenderInfo : RenderInfo
{
    Resources::Textures::Files texture = Resources::Textures::MissingTexture;
    FloatRect screenRect;
    FloatRect textureRect;
    float alpha = 1;
};

struct TextRenderInfo : RenderInfo
{
    DirectX::SimpleMath::Vector2 position;
    DirectX::SimpleMath::Color color;
    Resources::Fonts::Files font;
    const wchar_t * text = L"";
};


struct StaticRenderInfo : RenderInfo
{
    Resources::Models::Files model = Resources::Models::UnitCube;
    DirectX::SimpleMath::Matrix transform;
    DirectX::SimpleMath::Vector3 color = DirectX::SimpleMath::Color(1,1,1,1);
    bool useGridTexture = false;
};

struct FoliageRenderInfo : StaticRenderInfo
{
};

struct AnimatedRenderInfo : StaticRenderInfo
{
    const char * animationName = "";
    float animationTimeStamp = 0;
};

struct EnemyRenderInfo : AnimatedRenderInfo
{
    float freeze = 0;
    float burn = 0;
};

struct PlayerRenderInfo : AnimatedRenderInfo
{
};
