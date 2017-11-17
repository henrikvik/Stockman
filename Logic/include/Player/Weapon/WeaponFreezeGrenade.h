#ifndef WEAPONFREEZEGUN_H
#define WEAPONFREEZEGUN_H

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
        WeaponFreezeGrenade(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo, ProjectileData &freezeData, int splitCount);
        virtual ~WeaponFreezeGrenade();
    private:
        virtual void onUse(std::vector<Projectile*> &projectiles, Entity& shooter);

        ProjectileData* m_freezeData;
        int m_splitCount;
        float m_sliceSize;
    };
}

#endif