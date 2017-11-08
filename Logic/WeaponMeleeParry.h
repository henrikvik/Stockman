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
        virtual ~WeaponMeleeParry();

    private:
        void onUse(std::vector<Projectile*> &projectiles);
    };
}

#endif // !WEAPONMELEEPARRY_H




