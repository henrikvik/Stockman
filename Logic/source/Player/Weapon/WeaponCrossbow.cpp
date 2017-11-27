#include <Player\Weapon\WeaponCrossbow.h>
#include <Projectile\ProjectileStruct.h>

using namespace Logic;

WeaponCrossbow::WeaponCrossbow(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo)
    : Weapon(projectileManager, projectileData, wInfo)
{
}


WeaponCrossbow::~WeaponCrossbow()
{
}

bool WeaponCrossbow::useEnhanced(bool shouldUse)
{
    getProjectileData().hasEffect = shouldUse;

    if (shouldUse)
        getProjectileData().type = ProjectileTypeCrossbowFire;
    else
        getProjectileData().type = ProjectileTypeNormal;

    return shouldUse;
}
