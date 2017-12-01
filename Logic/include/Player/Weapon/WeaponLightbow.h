#pragma once

#include <Player\Weapon\Weapon.h>

namespace Logic
{
    class WeaponModel;

    struct LightDef {
        float color[3];
        float radius;
        float intensity;
    };
    struct LightEntry {
        float pos[3];
        int idx;
    };
    class WeaponLightbow : public Weapon
    {
    public:
        WeaponLightbow(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo);
        virtual ~WeaponLightbow();

        void addLight(int idx, Entity::CallbackData &data);
        virtual void update(float dt) override;
        virtual void onUse(std::vector<Projectile*> &projectiles, Entity& shooter);

        int m_CurrentIdx;
        std::vector<LightDef> m_LightDefs;
        std::vector<LightEntry> m_Entries;
    };
}



