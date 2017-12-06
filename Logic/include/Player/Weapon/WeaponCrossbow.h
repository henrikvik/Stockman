#ifndef WEAPONCROSSBOWPRIM_H
#define WEAPONCROSSBOWPRIM_H

#include <Player\Weapon\Weapon.h>

namespace Logic
{
    class WeaponCrossbow : public Weapon
    {
    public:
        WeaponCrossbow(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo, float baseDmg, float fireDmg);
        virtual ~WeaponCrossbow();

        virtual bool useEnhanced(bool shouldUse);

    private:
        float m_baseDmg;
        float m_fireDmg;
    };
}

#endif // !WEAPONCROSSBOWPRIM_H



