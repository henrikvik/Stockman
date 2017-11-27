#include <Player\Weapon\WeaponFreezeGun.h>
#include <Projectile\Projectile.h>
#include <AI\Enemy.h>

using namespace Logic;

WeaponFreezeGun::WeaponFreezeGun() { }

WeaponFreezeGun::WeaponFreezeGun(ProjectileManager * projectileManager, ProjectileData & projectileData, WeaponInfo wInfo)
    : Weapon(projectileManager, projectileData, wInfo) { }

WeaponFreezeGun::~WeaponFreezeGun() { }

void WeaponFreezeGun::onUse(std::vector<Projectile*>& projectiles, Entity& shooter)
{
    shooter.getSoundSource()->playSFX(Sound::SFX::WEAPON_ICEGUN_PRIMARY, 1.f, 0.15f);
}