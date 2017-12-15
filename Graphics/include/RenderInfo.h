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
struct InstanceCap { static constexpr size_t value = 1500; };
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
SET_INSTANCE_CAP(DebugRenderInfo, 100000)

struct NewDebugRenderInfo : RenderInfo
{
    struct Point
    {
        Point(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Color color) : position(position), color(color) {}
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Color color;
    };

    D3D11_PRIMITIVE_TOPOLOGY topology;
    DirectX::SimpleMath::Color color;
    std::vector<Point> * points;
    bool useDepth;
};
SET_INSTANCE_CAP(NewDebugRenderInfo, 100000)

struct LightRenderInfo : RenderInfo
{
    LightRenderInfo() : position(DirectX::SimpleMath::Vector3(0, 0, 0)), color(DirectX::SimpleMath::Color(1, 1, 1, 1)), intensity(1.f), range(3.f) { }
    LightRenderInfo(DirectX::SimpleMath::Color inColor, float inIntensity, float inRange) : position(DirectX::SimpleMath::Vector3(0, 0, 0)), color(inColor), intensity(inIntensity), range(inRange) { }

    DirectX::SimpleMath::Vector3 position;
    DirectX::SimpleMath::Color color;
    float intensity = 1;
    float range = 5;
};

SET_INSTANCE_CAP(LightRenderInfo, 254)

struct FancyRenderInfo : RenderInfo
{
    FancyRenderInfo() : position(DirectX::SimpleMath::Vector3{}), scale(1.f), color({ 1, 1, 1, 1 }), text(L"") { }

    std::wstring text;
    DirectX::SimpleMath::Vector3 position;
    float scale;
    DirectX::SimpleMath::Color color;
};

SET_INSTANCE_CAP(FancyRenderInfo, 254)

struct SpecialEffectRenderInfo : RenderInfo
{
    enum SpecialEffect { BulletTime, Snow, screenShake, DoF, screenBounce, Tint };
    SpecialEffect type;
    DirectX::SimpleMath::Vector2 direction;
    DirectX::SimpleMath::Vector3 color;
    float progress = 0;
    float duration = 0;
    float radius = 0;
    float bounceMax = 0;
    bool restart = 0;
    bool affectEveryThing = 0;
};

struct SpriteRenderInfo : RenderInfo
{
    Resources::Textures::Files texture = Resources::Textures::MissingTexture;
    FloatRect screenRect;
    FloatRect textureRect;
    float alpha = 1;
    bool isMoveable = 0;
};

struct TextRenderInfo : RenderInfo
{
    DirectX::SimpleMath::Vector2 position;
    DirectX::SimpleMath::Color color;
    Resources::Fonts::Files font;
    std::wstring text = L"";
    bool isMoveable = 0;
};


struct StaticRenderInfo : RenderInfo
{
    Resources::Models::Files model = Resources::Models::UnitCube;
    DirectX::SimpleMath::Matrix transform;
    DirectX::SimpleMath::Vector3 color = DirectX::SimpleMath::Color(1,1,1,1);
    bool useGridTexture = false;
    float cull_radius = 1.0;
};

struct FoliageRenderInfo : StaticRenderInfo
{
};

struct AnimatedRenderInfo : StaticRenderInfo
{
    const char * animationName = "";
    float animationTimeStamp = 0;
};

struct NewAnimatedRenderInfo : StaticRenderInfo
{
    std::vector<DirectX::SimpleMath::Matrix> * joint_transforms;
};

struct EnemyRenderInfo : AnimatedRenderInfo
{
    float freeze = 0;
    float burn = 0;
};

struct PlayerRenderInfo : AnimatedRenderInfo
{
};
