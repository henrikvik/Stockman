#ifndef WEAPONMELEEPARRY_H
#define WEAPONMELEEPARRY_H

#include <Player\Weapon\Weapon.h>
#include <vector>

namespace Logic
{
    class WeaponMeleeParry : public Weapon
    {
    public:
        WeaponMeleeParry();
        WeaponMeleeParry(ProjectileManager * projectileManager, ProjectileData & projectileData, WeaponInfo wInfo, float knockbackPower);
        virtual ~WeaponMeleeParry();

    private:
        void onUse(std::vector<Projectile*> &projectiles, Entity& shooter);

        float m_knockbackPower;
    };
}

#endif // !WEAPONMELEEPARRY_H