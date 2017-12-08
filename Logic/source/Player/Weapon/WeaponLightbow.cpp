#include <Player\Weapon\WeaponLightbow.h>
#include <Projectile\ProjectileStruct.h>
#include <Projectile\Projectile.h>

#include <Graphics\include\Utility\DebugDraw.h>

#include <toml\toml.h>

#include <Mouse.h>

namespace Logic {

WeaponLightbow::WeaponLightbow(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo)
    : Weapon(projectileManager, projectileData, wInfo),
        m_CurrentIdx(0)
{
}


WeaponLightbow::~WeaponLightbow()
{
}

void WeaponLightbow::addLight(int idx, Entity::CallbackData &data)
{
    printf("%d\n", idx);

    auto pos = data.caller->getPosition();
    m_Entries.push_back(LightEntry{
        { pos.x, pos.y, pos.z },
        idx
    });
}
float f = 0.f;
void WeaponLightbow::update(float dt) {
    f += dt / 1000.f;

    ImGui::Begin("Lightbow Interface");
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

        root.write(&std::cout);
    }

    if (!m_LightDefs.empty())
        ImGui::DragInt("Light Ammo Type", &m_CurrentIdx, 1.f, 0, m_LightDefs.size() - 1);
    if (ImGui::Button("Add Ammo")) {
        LightDef def = {};
        m_LightDefs.push_back(def);
    }

    ImGui::Separator();

    ImGui::DragInt("Selected Light", &m_SelectedLight, 1.f, 0, m_Entries.size() - 1);
    if (m_SelectedLight > 0 && m_SelectedLight < m_Entries.size()) {
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

    if (sz > 0) {
        for (auto proj : projectiles) {
            proj->addCallback(Entity::ON_COLLISION, [=](Entity::CallbackData &data) -> void {
                printf("Added light!\n");
                addLight(idx, data);
            });
        }
    }
}

}