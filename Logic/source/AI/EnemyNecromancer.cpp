#include <AI\EnemyNecromancer.h>
#include <AI\Behavior\RangedBehavior.h>
#include <Misc\RandomGenerator.h>

using namespace Logic;

EnemyNecromancer::EnemyNecromancer(Graphics::ModelID modelID,
	btRigidBody* body, btVector3 halfExtent)
	: Enemy(modelID, body, halfExtent, 5, 5, 0, 0) {
	setBehavior(RANGED);
}

EnemyNecromancer::~EnemyNecromancer()
{
}

void EnemyNecromancer::clear()
{
}

void EnemyNecromancer::onCollision(Entity & other)
{
	if (Projectile *pj = dynamic_cast<Projectile*> (&other))
		if (!pj->getProjectileData().enemyBullet)
			damage(pj->getProjectileData().damage);
}

void EnemyNecromancer::onCollision(Player & other)
{

}

void EnemyNecromancer::updateSpecific(Player const & player, float deltaTime)
{
}

void EnemyNecromancer::updateDead(float deltaTime)
{
}

void EnemyNecromancer::useAbility(Entity const &target)
{
	if (RandomGenerator::singleton().getRandomInt(0, 1))
	{
		spawnProjectile((target.getPositionBT() - getPositionBT()).normalize(), Graphics::ModelID::GRASS, 150.f);
	}
	else
	{
		spawnProjectile((target.getPositionBT() - getPositionBT()).normalize(), Graphics::ModelID::ENEMYGRUNT, 150.f);
	}
}
