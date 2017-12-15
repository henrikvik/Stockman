#ifndef WEAPONFREEZEGRENADE_H
#define WEAPONFREEZEGRENADE_H

#include <Player\Weapon\Weapon.h>
#include <vector>

namespace Logic
{
    class Projectile;
    struct ProjectileData;

    class WeaponFreezeGrenade : public Weapon
    {
    public:
        WeaponFreezeGrenade();
        WeaponFreezeGrenade(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo, ProjectileData &freezeData, ProjectileData& explosionData, int splitCount);
        virtual ~WeaponFreezeGrenade();
    private:
        virtual void onUse(std::vector<Projectile*> &projectiles, Entity& shooter);
        void doExplosionCallback(Entity::CallbackData &data);
        void doExplosionCallbackEnhanced(Entity::CallbackData &data);
        virtual bool useEnhanced(bool shouldUse);

        ProjectileData m_freezeData;
        ProjectileData m_explosionData;
        int m_splitCount;
        float m_sliceSize;
        bool m_enhanced;
    };
}

#endif //!WEAPONFREEZEGRENADE_H