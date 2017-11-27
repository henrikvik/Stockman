#include <Player\Weapon\WeaponMeleeSwing.h>
#include <Projectile\Projectile.h>
#include <AI\Enemy.h>

using namespace Logic;

WeaponMeleeSwing::WeaponMeleeSwing() : m_knockbackPower(0.f) { }

WeaponMeleeSwing::WeaponMeleeSwing(ProjectileManager * projectileManager, ProjectileData & projectileData, WeaponInfo wInfo, float knockbackPower)
    : Weapon(projectileManager, projectileData, wInfo)
{
    m_knockbackPower = knockbackPower;
}

WeaponMeleeSwing::~WeaponMeleeSwing() { }

void WeaponMeleeSwing::onUse(std::vector<Projectile*>& projectiles, Entity& shooter)
{
    shooter.getSoundSource()->playSFX(Sound::SFX::WEAPON_MELEE_PRIMARY);

    // Knockback on hitting enemies here.
}