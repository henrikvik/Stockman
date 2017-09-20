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
}

void EnemyTest::onCollision(Player& other) 
{
	
}

void EnemyTest::updateSpecific(float deltaTime)
{
	
}

void EnemyTest::updateDead(float deltaTime)
{
	// x _ x
}
