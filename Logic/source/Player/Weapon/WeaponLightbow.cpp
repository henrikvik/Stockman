#include <Player\Weapon\WeaponLightbow.h>
#include <Projectile\ProjectileStruct.h>
#include <Projectile\Projectile.h>

#include <Graphics\include\Utility\DebugDraw.h>

#include <toml\toml.h>

#include <Mouse.h>

static void FillLightVec(std::vector<LightRenderInfo> &lights, std::string path)
{
    auto val = toml::parseFile(path).value;

    for (auto light : val.find("lights")->as<toml::Array>()) {
        auto position = light["position"].as<toml::Array>();
        auto col = light["color"].as<toml::Array>();
        auto radius = light["range"].asNumber();
        auto intensity = light["intensity"].asNumber();

        LightRenderInfo info = {};
        info.position.x = position[0].asNumber();
        info.position.y = position[1].asNumber();
        info.position.z = position[2].asNumber();
        info.color.x = col[0].asNumber();
        info.color.y = col[1].asNumber();
        info.color.z = col[2].asNumber();
        info.range = radius;
        info.intensity = intensity;

        lights.push_back(info);
    }
}

static void SubmitLights(std::vector<LightRenderInfo> &lights)
{
    for (auto light : lights) {
        QueueRender(light);
    }
}

namespace Logic {



WeaponLightbow::WeaponLightbow(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo)
    : Weapon(projectileManager, projectileData, wInfo),
        m_CurrentIdx(0), m_SelectedLight(0), m_ChristmasTimer(3.f), m_ChristmasIndex(0)
{
    FillLightVec(m_Lanterns, "lyktor.toml");
    FillLightVec(m_Red, "r.toml");
    FillLightVec(m_Green, "g.toml");
    FillLightVec(m_Blue, "b.toml");

    m_ChristmasPattern.push_back(0b00000001);
    m_ChristmasPattern.push_back(0b00000010);
    m_ChristmasPattern.push_back(0b00000100);
    m_ChristmasPattern.push_back(0b00000010);
    m_ChristmasPattern.push_back(0b00000001);
    m_ChristmasPattern.push_back(0b00000011);
    m_ChristmasPattern.push_back(0b00000111);
    m_ChristmasPattern.push_back(0b00000011);
}


WeaponLightbow::~WeaponLightbow()
{
}

void WeaponLightbow::addLight(int idx, Entity::CallbackData &data)
{
    auto pos = data.caller->getPosition();
    m_Entries.push_back(LightEntry{
        { pos.x, pos.y, pos.z },
        idx
    });
}
float f = 0.f;
void WeaponLightbow::update(float dt) {
    f += dt / 1000.f;
    static bool lanterns = true;

    if (f > m_ChristmasTimer) {
        f = 0.f;
        m_ChristmasIndex = (m_ChristmasIndex + 1) % m_ChristmasPattern.size();
    }
    
    auto pattern = m_ChristmasPattern[m_ChristmasIndex];

    if (pattern & 0x1)
        SubmitLights(m_Red);
    if (pattern & 0x2)
        SubmitLights(m_Green);
    if (pattern & 0x4)
        SubmitLights(m_Blue);

    if (lanterns) {
        SubmitLights(m_Lanterns);
    }

    ImGui::Begin("Lightbow Interface");

    ImGui::DragFloat("christmas!!", &m_ChristmasTimer, 1.f, 0.05f, 10.f);
    if (ImGui::Button("Dump")) {
        auto root = toml::Value(toml::Table());
        
        for (auto &entry : m_Entries) {
            auto val = toml::Table();
            auto def = m_LightDefs[entry.idx];

            val["position"] = toml::Array({ entry.pos[0], entry.pos[1], entry.pos[2] });
            val["color"] = toml::Array({ def.color[0], def.color[1], def.color[2] });
            val["range"] = def.radius;
            val["intensity"] = def.intensity;

            root["lights"].push(val);
        }

        std::ofstream lightfile("lights.toml");
        root.write(&lightfile);
    }

    if (!m_LightDefs.empty())
        ImGui::DragInt("Light Ammo Type", &m_CurrentIdx, 1.f, 0, m_LightDefs.size() - 1);
    if (ImGui::Button("Add Ammo")) {
        LightDef def = {};
        m_LightDefs.push_back(def);
    }

    ImGui::Separator();

    ImGui::DragInt("Selected Light", &m_SelectedLight, 1.f, 0, m_Entries.size() - 1);
    if (!m_Entries.empty() && m_SelectedLight >= 0 && m_SelectedLight < m_Entries.size()) {
        if (ImGui::Button("Delete")) {
            m_Entries.erase(m_Entries.begin() + m_SelectedLight);
        }
        ImGui::DragInt("type", &m_Entries[m_SelectedLight].idx, 1.f, 0, m_LightDefs.size() - 1);
        ImGui::DragFloat3("position", m_Entries[m_SelectedLight].pos);
    }
    
    ImGui::Separator();

    int i = 0;
    for (auto &def : m_LightDefs) {
        ImGui::TextDisabled("%d", i);
        char fmt[128] = { 0 };
        sprintf_s(fmt, "Color##%d", i);
        ImGui::ColorEdit3(fmt, def.color, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float);
        sprintf_s(fmt, "Radius##%d", i);
        ImGui::SliderFloat(fmt, &def.radius, 0.f, 20.f);
        sprintf_s(fmt, "Intensity##%d", i);
        ImGui::SliderFloat(fmt, &def.intensity, 0.f, 10.f);
        i++;
    }
    ImGui::End();

    LightRenderInfo info;
    i = 0;
    for (auto &entry : m_Entries) {
        
        auto def = m_LightDefs[entry.idx];
        info.color.x = def.color[0];
        info.color.y = def.color[1];
        info.color.z = def.color[2];
        info.range = def.radius;
        info.intensity = def.intensity;
        info.position.x = entry.pos[0];
        info.position.y = entry.pos[1];
        info.position.z = entry.pos[2];

        Graphics::Debug::Text(info.position, i == m_SelectedLight ? ImColor(1.f, 0.8f, 0.1f) : ImColor(0xffffffff), std::to_string(i));

        QueueRender(info);
        i++;
    }
}


void WeaponLightbow::onUse(std::vector<Projectile*> &projectiles, Entity& shooter)
{
    auto idx = m_CurrentIdx;
    auto sz = m_LightDefs.size();

    printf("shot light!\n");
    if (sz > 0) {
        for (auto proj : projectiles) {
            proj->addCallback(Entity::ON_COLLISION, [=](Entity::CallbackData &data) -> void {
                printf("  Added light!\n");
                addLight(idx, data);
            });
        }
    }
}

}