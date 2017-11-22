#ifndef WEAPONCROSSBOWPRIM_H
#define WEAPONCROSSBOWPRIM_H

#include <Player\Weapon\Weapon.h>

namespace Logic
{
    class WeaponCrossbow : public Weapon
    {
    public:
        WeaponCrossbow(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo);
        virtual ~WeaponCrossbow();

        virtual bool useEnhanced(bool shouldUse);
    };
}

#endif // !WEAPONCROSSBOWPRIM_H



