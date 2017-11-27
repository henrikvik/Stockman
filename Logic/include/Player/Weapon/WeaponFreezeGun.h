#ifndef WEAPONFREEZEGUN_H
#define WEAPONFREEZEGUN_H

#include <Player\Weapon\Weapon.h>

namespace Logic
{
    class WeaponFreezeGun : public Weapon
    {
    public:
        WeaponFreezeGun();
        WeaponFreezeGun(ProjectileManager * projectileManager, ProjectileData & projectileData, WeaponInfo wInfo);
        ~WeaponFreezeGun();

    private:
        void onUse(std::vector<Projectile*> &projectiles, Entity& shooter);
    };
}

#endif // !WEAPONFREEZEGUN_H