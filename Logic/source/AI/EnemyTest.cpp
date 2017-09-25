#include "AI/EnemyTest.h"
#include <AI\Behavior\TestBehavior.h>
using namespace Logic;

EnemyTest::EnemyTest(btRigidBody* body, btVector3 halfExtent)
: Enemy(body, halfExtent, 10, 5, 3, 1) { //just test values
	setBehavior(TEST);
}


EnemyTest::~EnemyTest()
{
}

void EnemyTest::clear()
{
}

void EnemyTest::onCollision(Entity &other)
{
	if (Projectile *p = dynamic_cast<Projectile*> (&other)) {
		damage(p->getDamage());
		btVector3 dir = other.getRigidbody()->getLinearVelocity();
		dir = dir.normalize();
		dir *= 1000.f;
		getRigidbody()->applyCentralForce(dir);
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
	// use the behavoiur to use shareable move patterns
}

void EnemyTest::updateDead(float deltaTime)
{
	Entity::updateGraphics();
	// x _ x
}
