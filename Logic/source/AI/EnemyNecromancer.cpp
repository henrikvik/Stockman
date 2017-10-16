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
			spawnProjectile((target.getPositionBT() - getPositionBT()).normalize(), Graphics::ModelID::GRASS, SPEED_AB2);
		}
		else
		{
			spawnProjectile((target.getPositionBT() - getPositionBT()).normalize(), Graphics::ModelID::ENEMYGRUNT, SPEED_AB1);
		}
	}
}
