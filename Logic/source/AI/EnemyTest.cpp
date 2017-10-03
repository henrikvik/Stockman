#include "AI/EnemyTest.h"
#include <AI\Behavior\TestBehavior.h>
using namespace Logic;

EnemyTest::EnemyTest(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent)
: Enemy(modelID, body, halfExtent, 10, 5, 3, 1) { //just test values
	setBehavior(TEST);
}


EnemyTest::~EnemyTest()
{
}

void EnemyTest::clear()
{
}

void EnemyTest::onCollision(Entity &other, const btRigidBody* collidedWithYour)
{
	if (Projectile *p = dynamic_cast<Projectile*> (&other)) {
		if (!p->getProjectileData().enemyBullet)
		{
			damage(p->getProjectileData().damage);
			btVector3 dir = other.getRigidbody()->getLinearVelocity();
			dir = dir.normalize();
			dir *= 1000.f;
			getRigidbody()->applyCentralForce(dir);
		}
	} if (Player *p = dynamic_cast<Player*> (&other))
		onCollision(*p);
}

void EnemyTest::onCollision(Player& other) 
{
	btVector3 dir = getPositionBT() - other.getPositionBT();
	dir = dir.normalize();
	dir *= 100000.f;
	getRigidbody()->applyCentralForce(dir);
}

void EnemyTest::updateSpecific(Player const &player, float deltaTime)
{
	// use the behavior to use shareable patterns
}

void EnemyTest::updateDead(float deltaTime)
{
	Entity::updateGraphics();
	// x _ x
}
