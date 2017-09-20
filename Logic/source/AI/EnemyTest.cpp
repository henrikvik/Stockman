#include "AI/EnemyTest.h"
using namespace Logic;

EnemyTest::EnemyTest(btRigidBody* body, btVector3 halfExtent)
: Enemy(body, halfExtent, 10, 5, 3, 1) { //just test values
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
		//damage(1);
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
	btVector3 dir = player.getPositionBT() - getPositionBT();
	dir = dir.normalize();
	dir *= deltaTime / 1000.f;
	dir *= 15;
	getRigidbody()->translate(dir);
//	getRigidbody()->applyCentralForce(dir);
}

void EnemyTest::updateDead(float deltaTime)
{
	Entity::updateGraphics();
	// x _ x
}
