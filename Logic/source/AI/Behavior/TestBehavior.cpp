#include <AI\Behavior\TestBehavior.h> 
#include "../../../include/AI/Enemy.h"
using namespace Logic;

TestBehavior::~TestBehavior()
{
}

void TestBehavior::update(Enemy &enemy, Player const & player, float deltaTime)
{
	btVector3 dir = AStar::singleton().getNextNode(enemy, player).position
		- enemy.getPositionBT();

	dir = dir.normalize();
	dir *= deltaTime / 1000.f;
	dir *= 40;

	if (enemy.getHealth() < 5)
	{
		enemy.getRigidbody()->applyCentralForce(dir * -20000);
	}
		else 
	{
		enemy.getRigidbody()->translate(dir);
	}
}
