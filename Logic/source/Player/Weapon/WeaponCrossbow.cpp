#include <Player\Weapon\WeaponCrossbow.h>
#include <Projectile\ProjectileStruct.h>
#include <Mouse.h>

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
    if (DirectX::Mouse::Get().GetState().leftButton) Sound::NoiseMachine::Get().playSFX(Sound::SFX::WEAPON_CUTLERY_PRIMARY, nullptr, true);
    else Sound::NoiseMachine::Get().playSFX(Sound::SFX::WEAPON_CUTLERY_SECONDARY, nullptr, true);

    getProjectileData().hasEffect = shouldUse;

    if (shouldUse)
        getProjectileData().type = ProjectileTypeCrossbowFire;
    else
        getProjectileData().type = ProjectileTypeNormal;

    return true;
}
