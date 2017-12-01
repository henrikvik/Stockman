#include <Player\Weapon\WeaponLightbow.h>
#include <Projectile\ProjectileStruct.h>
#include <Projectile\Projectile.h>

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

    ImGui::DragInt("Light Ammo Type", &m_CurrentIdx, 1.f, 0, m_LightDefs.size() - 1);
    if (ImGui::Button("Add Ammo")) {
        LightDef def = {};
        m_LightDefs.push_back(def);
    }

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

        QueueRender(info);
    }
}


void WeaponLightbow::onUse(std::vector<Projectile*> &projectiles, Entity& shooter)
{
    auto idx = m_CurrentIdx;
    auto sz = m_LightDefs.size();

    if (sz > 0) {
        for (auto proj : projectiles) {
            proj->addCallback(Entity::ON_COLLISION, [=](Entity::CallbackData &data) -> void {
                printf("add lights!\n");
                addLight(idx, data);
            });
        }
    }
}

}