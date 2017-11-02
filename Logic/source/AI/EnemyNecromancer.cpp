#include <AI\EnemyNecromancer.h>
#include <AI\Behavior\RangedBehavior.h>
#include <Misc\RandomGenerator.h>
#include <Projectile\Projectile.h>
#include <Misc\ComboMachine.h>

using namespace Logic;

EnemyNecromancer::EnemyNecromancer(btRigidBody* body, btVector3 halfExtent)
	: Enemy(Resources::Models::Files::UnitCube, body, halfExtent, 5, 1, 8, NECROMANCER, 0)
{
	setBehavior(RANGED);
    addCallback(ON_DEATH, [&](CallbackData data) -> void {
        ComboMachine::Get().Kill(getEnemyType());
        SpawnTrigger(2, getPositionBT(), std::vector<int>{ StatusManager::AMMO_PICK_UP_PRIMARY });
    });
    m_spawnedMinions = 0;
}

EnemyNecromancer::~EnemyNecromancer()
{
}

void EnemyNecromancer::clear()
{
}

void EnemyNecromancer::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
	if (Enemy *e = dynamic_cast<Enemy*>(&other))
	{
	}
	else if (Projectile *pj = dynamic_cast<Projectile*> (&other))
	{
        if (!pj->getProjectileData().enemyBullet)
        {
            damage(pj->getProjectileData().damage * dmgMultiplier);

            if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
        }
	}
}

void EnemyNecromancer::onCollision(Player &other)
{

}

void EnemyNecromancer::updateSpecific(Player const & player, float deltaTime)
{
}

void EnemyNecromancer::updateDead(float deltaTime)
{
	Entity::update(deltaTime);
}

void EnemyNecromancer::useAbility(Entity const &target)
{
	if (RandomGenerator::singleton().getRandomInt(0, 800))
	{
		if (m_spawnedMinions < MAX_SPAWNED_MINIONS)
		{
            Projectile *pj = shoot(((target.getPositionBT() - getPositionBT()) + btVector3{0, 80, 0}).normalize(), Resources::Models::UnitCube, SPEED_AB2, 2, 0.4);
            pj->addCallback(ON_COLLISION, [&](CallbackData &data) -> void {
                Entity *entity = reinterpret_cast<Entity*> (data.dataPtr);

                if (m_spawnedMinions < MAX_SPAWNED_MINIONS)
                {
                    Enemy *e = SpawnEnemy(ENEMY_TYPE::NECROMANCER_MINION, data.caller->getPositionBT(), {});
                    m_spawnedMinions++;

                    e->addCallback(ON_DEATH, [&](CallbackData data) -> void {
                        m_spawnedMinions--;
                    });
                }
            });
		}
		else
		{
            shoot((target.getPositionBT() - getPositionBT()).normalize(), Resources::Models::UnitCube, SPEED_AB1, 1.1, 0.2);
		}
	}
}
