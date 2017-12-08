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
        int m_SelectedLight;
        std::vector<LightDef> m_LightDefs;
        std::vector<LightEntry> m_Entries;

        std::vector<char> m_ChristmasPattern;
        int m_ChristmasIndex;
        float m_ChristmasTimer;

        std::vector<LightRenderInfo> m_Red;
        std::vector<LightRenderInfo> m_Green;
        std::vector<LightRenderInfo> m_Blue;

        std::vector<LightRenderInfo> m_Lanterns;
    };
}



