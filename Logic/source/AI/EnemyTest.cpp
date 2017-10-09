#include "AI/EnemyTest.h"
#include <AI\Behavior\TestBehavior.h>
using namespace Logic;

EnemyTest::EnemyTest(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent)
: Enemy(modelID, body, halfExtent, 10, 5, 5, 3, 1) { //just test values
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
			if (p->getProjectileData().type == ProjectileType::ProjectileTypeBulletTimeSensor)
				setStatusManager(p->getStatusManager()); // TEMP
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
