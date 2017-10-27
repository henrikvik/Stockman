#include <AI\EnemyNecromancer.h>
#include <AI\Behavior\RangedBehavior.h>
#include <Misc\RandomGenerator.h>

using namespace Logic;

EnemyNecromancer::EnemyNecromancer(Graphics::ModelID modelID,
	btRigidBody* body, btVector3 halfExtent)
	: Enemy(modelID, body, halfExtent, 5, 1, 8, 0, 0) {
	setBehavior(RANGED);
    addCallback(ON_DEATH, [&](CallbackData data) -> void {
        SpawnTrigger(1, getPositionBT(), std::vector<int>{ StatusManager::AMMO_PICK_UP });
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
            if (dmgMultiplier > 1.01f)
                printf("HS.");
        }

		if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
			getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
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
	if (RandomGenerator::singleton().getRandomInt(0, 1000))
	{
		if (m_spawnedMinions < MAX_SPAWNED_MINIONS)
		{
            Projectile *pj = shoot(((target.getPositionBT() - getPositionBT()) + btVector3{0, 80, 0}).normalize(), Graphics::ModelID::SKY_SPHERE, SPEED_AB2);
            pj->addCallback(ON_COLLISION, [&](CallbackData &data) -> void {
                Entity *entity = reinterpret_cast<Entity*> (data.dataPtr);
                std::vector<int> effects = { StatusManager::EFFECT_ID::AMMO_PICK_UP };

                if (m_spawnedMinions < MAX_SPAWNED_MINIONS)
                {
                    Enemy *e = SpawnEnemy(data.caller->getPositionBT(), ENEMY_TYPE::NECROMANCER_MINION);
                    m_spawnedMinions++;

                    e->addCallback(ON_DEATH, [&](CallbackData data) -> void {
                        m_spawnedMinions--;
                    });
                }
            });
		}
		else
		{
            shoot((target.getPositionBT() - getPositionBT()).normalize(), Graphics::ModelID::ENEMYGRUNT, SPEED_AB1);
		}
	}
}
