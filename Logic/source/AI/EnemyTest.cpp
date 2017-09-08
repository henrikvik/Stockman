#include "AI/EnemyTest.h"
using namespace Logic;

EnemyTest::EnemyTest()
: Enemy(10, 5, 3, 1) { //just test values
}


EnemyTest::~EnemyTest()
{
}

void EnemyTest::clear()
{
}

void EnemyTest::onCollision(const Entity &other)
{
}

void EnemyTest::updateSpec(float deltaTime) 
{
	int i = 5000 * 7000;
	if (i / 3 / 2 / 5 / 3 == 0) i = 3; // pressure
}

void EnemyTest::updateDead(float deltaTime)
{
	// x _ x
}
