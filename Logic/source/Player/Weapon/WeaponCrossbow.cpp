#include <Player\Weapon\WeaponCrossbow.h>
#include <Projectile\ProjectileStruct.h>
#include <Mouse.h>

using namespace Logic;

WeaponCrossbow::WeaponCrossbow(ProjectileManager* projectileManager, ProjectileData &projectileData, WeaponInfo wInfo, float baseDmg, float fireDmg)
    : Weapon(projectileManager, projectileData, wInfo)
{
    m_baseDmg = baseDmg;
    m_fireDmg = fireDmg;
}


WeaponCrossbow::~WeaponCrossbow()
{
}

bool WeaponCrossbow::useEnhanced(bool shouldUse)
{
    if (DirectX::Mouse::Get().GetState().leftButton) Sound::NoiseMachine::Get().playSFX(Sound::SFX::WEAPON_CUTLERY_PRIMARY, nullptr, true);
    else Sound::NoiseMachine::Get().playSFX(Sound::SFX::WEAPON_CUTLERY_SECONDARY, nullptr, true);

    getProjectileData().hasEffect = shouldUse;

    ProjectileData& pData = getProjectileData();
    if (shouldUse)
    {
        pData.damage = m_fireDmg;
        pData.type = ProjectileTypeFireArrow;
    }
    else
    {
        pData.damage = m_baseDmg;
        pData.type = ProjectileTypeNormal;
    }

    return shouldUse;
}
