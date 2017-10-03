#include "AI/EnemyTest.h"
#include <AI\Behavior\TestBehavior.h>
using namespace Logic;

EnemyTest::EnemyTest(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent)
: Enemy(modelID, body, halfExtent, 10, 5, 15, 3, 1) { //just test values
	setBehavior(TEST);
}


EnemyTest::~EnemyTest()
{
}

void EnemyTest::clear()
{
}

void EnemyTest::onCollision(Entity &other, btVector3 collPoint)
{
	if (Projectile *p = dynamic_cast<Projectile*> (&other)) {
		damage(p->getDamage());
		btVector3 dir = other.getRigidbody()->getLinearVelocity();
		dir = dir.normalize();
		dir *= 1000.f;
		getRigidbody()->applyCentralForce(dir);

		// BULLET TIME
		if (p->getType() == ProjectileType::ProjectileTypeBulletTimeSensor)
			getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, 1);
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
