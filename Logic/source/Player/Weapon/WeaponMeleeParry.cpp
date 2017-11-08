#include <Player\Weapon\WeaponMeleeParry.h>
#include <Projectile\Projectile.h>
#include <AI\Enemy.h>

using namespace Logic;

WeaponMeleeParry::WeaponMeleeParry()
{
}

WeaponMeleeParry::WeaponMeleeParry(ProjectileManager * projectileManager, ProjectileData & projectileData, WeaponInfo wInfo, float knockbackPower)
    : Weapon(projectileManager, projectileData, wInfo)
{
    m_knockbackPower = knockbackPower;
}

WeaponMeleeParry::~WeaponMeleeParry()
{
}

void WeaponMeleeParry::onUse(std::vector<Projectile*>& projectiles)
{
    for (Projectile* p : projectiles)
    {
        p->addCallback(Entity::ON_COLLISION, [&](Entity::CallbackData &data) -> void {

            PhysicsObject* obj = reinterpret_cast<PhysicsObject*>(data.dataPtr);

            if (Enemy* enemy = dynamic_cast<Enemy*>(obj))
            {
                obj->getRigidBody()->setLinearVelocity(btVector3(0.f, 1.f, 0.f) * m_knockbackPower);
                enemy->getStatusManager().addStatus(StatusManager::EFFECT_ID::STUN, 1); // 10 second stun OP AF
            }

            
        });
    }
}
