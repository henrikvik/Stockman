#include "DebugDraw.h"

#include <WICTextureLoader.h>
#include <imgui.h>
#include "Buffer.h"
#include <Singeltons\DebugWindow.h>

static std::vector<Graphics::Light> DebugLights;
static std::vector<Graphics::ParticleEffectInstance> DebugFX;
static ID3D11ShaderResourceView *DebugLightSprite;
static ID3D11ShaderResourceView *DebugFXSprite;

static bool DebugDrawFX = false;
static bool DebugDrawLights = false;

static DirectX::SimpleMath::Vector4 TransformScreen(DirectX::SimpleMath::Matrix vp, DirectX::SimpleMath::Vector3 position, bool *clip)
{
    using namespace DirectX::SimpleMath;

    auto vec = Vector4(position);
    vec.w = 1;
    auto pos = Vector4::Transform(vec, vp);

    // varför?
    if (pos.w == 0.f) pos.w = 1.f;

    pos /= pos.w;
    pos = (pos * 0.5f) + Vector4(0.5f);
    if (clip && pos.z > 1.f) *clip = true;
    pos.y = 1 - pos.y;
    pos.x *= WIN_WIDTH;
    pos.y *= WIN_HEIGHT;
    return pos;
}

void Graphics::Debug::Initialize(ID3D11Device * device)
{
    DirectX::CreateWICTextureFromFile(device, TEXTURE_PATH("debug_light.png"), NULL, &DebugLightSprite);
    DirectX::CreateWICTextureFromFile(device, TEXTURE_PATH("debug_fx.png"), NULL, &DebugFXSprite);
    
    DebugWindow::getInstance()->registerCommand("GFX_DEBUG_FX", [&](std::vector<std::string> &args)->std::string
    {
        DebugDrawFX = !DebugDrawFX;

        std::string response = "DebugDrawFX = ";
        response += DebugDrawFX ? "true" : "false";

        return response;
    });

    DebugWindow::getInstance()->registerCommand("GFX_DEBUG_LIGHTS", [&](std::vector<std::string> &args)->std::string
    {
        DebugDrawLights = !DebugDrawLights;

        std::string response = "DebugDrawLights = ";
        response += DebugDrawLights ? "true" : "false";

        return response;
    });
}

void Graphics::Debug::Destroy()
{
    SAFE_RELEASE(DebugLightSprite);
    SAFE_RELEASE(DebugFXSprite);
}

void Graphics::Debug::Text(DirectX::SimpleMath::Vector3 pos, const char * str)
{
}

void Graphics::Debug::ParticleFX(ParticleEffectInstance fx)
{
    if (!DebugDrawFX) return;

    DebugFX.push_back(fx);
}

void Graphics::Debug::PointLight(Light light)
{
    if (!DebugDrawLights) return;

    DebugLights.push_back(light);
}

void GRAPHICS_API Graphics::Debug::Render(Camera *camera)
{
    using namespace DirectX::SimpleMath;
    
    auto vp = camera->getView() * camera->getProj();
    
    bool open = false;
    ImGui::Begin("asdasd", &open, ImVec2(0, 0), 0.f, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    auto list = ImGui::GetWindowDrawList();
    
    list->PushClipRectFullScreen();
    for (auto light : DebugLights) {
        bool clip = false;
        auto pos = TransformScreen(vp, light.position, &clip);

        if (clip) continue;

        pos.x = roundf(pos.x);
        pos.y = roundf(pos.y);

        list->AddImage(DebugLightSprite, ImVec2(pos.x - 8, pos.y - 8), ImVec2(pos.x + 7, pos.y + 7));
        list->AddRect(ImVec2(pos.x-14, pos.y-14), ImVec2(pos.x+13, pos.y+13), ImColor(light.color.x, light.color.y, light.color.z), 3.5f);
    }

    for (auto fx : DebugFX) {
        bool clip = false;
        auto pos = TransformScreen(vp, fx.position, &clip);

        if (clip) continue;

        pos.x = roundf(pos.x);
        pos.y = roundf(pos.y);

        list->AddImage(DebugFXSprite, ImVec2(pos.x - 8, pos.y - 8), ImVec2(pos.x + 7, pos.y + 7));
        list->AddRect(ImVec2(pos.x - 14, pos.y - 14), ImVec2(pos.x + 13, pos.y + 13), ImColor(1.f, 1.f, 1.f), 3.5f);
        list->AddText(ImVec2(pos.x + 16, pos.y + 14 - 13), 0xffffffff, fx.effect.m_Name);
    
        auto progress = fx.effect.m_Age / fx.effect.m_Time;

        list->AddRectFilled(ImVec2(pos.x + 16, pos.y - 14), ImVec2(pos.x + 16 + 40 * progress, pos.y - 1), ImColor(progress, 1.f - progress, 1.f), 3.5f);
        list->AddRect(ImVec2(pos.x + 16, pos.y - 14), ImVec2(pos.x + 16 + 40, pos.y - 1), ImColor(1.f, 1.f, 1.f), 3.5f);
    }
    list->PopClipRect();
    ImGui::End();

    DebugLights.clear();
    DebugFX.clear();
}