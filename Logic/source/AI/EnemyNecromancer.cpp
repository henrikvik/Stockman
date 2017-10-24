#include <AI\EnemyNecromancer.h>
#include <AI\Behavior\RangedBehavior.h>
#include <Misc\RandomGenerator.h>

using namespace Logic;

EnemyNecromancer::EnemyNecromancer(Graphics::ModelID modelID,
	btRigidBody* body, btVector3 halfExtent)
	: Enemy(modelID, body, halfExtent, 5, 1, 15, 0, 0) {
	setBehavior(RANGED);
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
		if (dmgMultiplier > 1.01f && dmgMultiplier < 2.01f)
			printf("Headshot. 2X DMG.\n");

		if (!pj->getProjectileData().enemyBullet)
			damage(pj->getProjectileData().damage * dmgMultiplier);

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
		if (RandomGenerator::singleton().getRandomInt(0, 1))
		{
		    Projectile *pj = spawnProjectile((target.getPositionBT() - getPositionBT()).normalize(), Graphics::ModelID::SKY_SPHERE, SPEED_AB2);
            pj->addCallback(ON_COLLISION, [](CallbackData &data) -> void {
                reinterpret_cast<Entity*> (data.dataPtr)->
                    getRigidBody()->applyCentralForce({ 0, 10000, 0 });
            });
		}
		else
		{
			spawnProjectile((target.getPositionBT() - getPositionBT()).normalize(), Graphics::ModelID::ENEMYGRUNT, SPEED_AB1);
		}
	}
}
