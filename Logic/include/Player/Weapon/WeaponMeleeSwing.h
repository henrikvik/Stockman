#ifndef WEAPONMELEESWING_H
#define WEAPONMELEESWING_H

#include <Player\Weapon\Weapon.h>

namespace Logic
{
    class WeaponMeleeSwing : public Weapon
    {
    public:
        WeaponMeleeSwing();
        WeaponMeleeSwing(ProjectileManager * projectileManager, ProjectileData & projectileData, WeaponInfo wInfo, float knockbackPower);
        virtual ~WeaponMeleeSwing();

    private:
        void onUse(std::vector<Projectile*> &projectiles, Entity& shooter);
        float m_knockbackPower;
    };
}

#endif // !WEAPONMELEESWING_H
