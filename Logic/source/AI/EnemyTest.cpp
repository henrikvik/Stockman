#include "AI/EnemyTest.h"
#include <AI\Behavior\TestBehavior.h>
#include <Projectile\Projectile.h>
using namespace Logic;

EnemyTest::EnemyTest(btRigidBody* body, btVector3 halfExtent)
: Enemy(Resources::Models::UnitCube, body, halfExtent, 10, 1, 5, NECROMANCER, 1) { //just test values
	setBehavior(TEST);
}


EnemyTest::~EnemyTest()
{
}

void EnemyTest::clear()
{
}

void EnemyTest::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
	if (Projectile *p = dynamic_cast<Projectile*> (&other)) {
		if (!p->getProjectileData().enemyBullet)
		{
			damage(p->getProjectileData().damage);

			// BULLET TIME
			if (p->getProjectileData().type == ProjectileTypeBulletTimeSensor)
				getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, p->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
		}
	}
	if (Player *p = dynamic_cast<Player*> (&other))
	{
		p->takeDamage(getBaseDamage());
	}
}

void EnemyTest::onCollision(Player& other) 
{
	btVector3 dir = getPositionBT() - other.getPositionBT();
	dir = dir.normalize();
	dir *= 100000.f;
	getRigidBody()->applyCentralForce(dir);
}

void EnemyTest::updateSpecific(Player const &player, float deltaTime)
{
	// use the behavior to use shareable patterns
}

void EnemyTest::updateDead(float deltaTime)
{

}
