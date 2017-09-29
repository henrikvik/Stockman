#include <AI\Behavior\TestBehavior.h> 
#include <AI\Enemy.h>
using namespace Logic;

TestBehavior::TestBehavior()
{
	currentNode = -1;
}

TestBehavior::~TestBehavior()
{
}

void TestBehavior::update(Enemy &enemy, Player const & player, float deltaTime)
{
	if (path.empty() || currentNode == path.size() - 1)
	{
		path = AStar::singleton().getNextNode(enemy, player);
		currentNode = 0;
	}

	const DirectX::SimpleMath::Vector3 *vec = path[currentNode];

	btVector3 node = { vec->x, vec->y, vec->z };
	btVector3 dir = node - enemy.getPositionBT();

	dir = dir.normalize();
	dir *= deltaTime / 1000.f;
	dir *= 400;

	if (enemy.getHealth() < 5)
	{
		enemy.getRigidbody()->applyCentralForce(dir * -20000);
	}
		else 
	{
		enemy.getRigidbody()->translate(dir);
	}

	if ((*vec - enemy.getPosition()).Length() < 2)
		currentNode++;
}
