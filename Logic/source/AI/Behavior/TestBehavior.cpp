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
	if (path.empty() || currentNode == path.size())
	{
		path = AStar::singleton().getPath(enemy, player);
		currentNode = 0;
	}

	btVector3 node;
	if (path.empty())
		node = player.getPositionBT();
	else {
		const DirectX::SimpleMath::Vector3 *vec = path[currentNode];
		node = { vec->x, vec->y, vec->z };
	}
	btVector3 dir = node - enemy.getPositionBT();

	dir = dir.normalize();
	dir *= deltaTime / 1000.f;
	dir *= 65;

	if (enemy.getHealth() < 5)
	{
		enemy.getRigidbody()->applyCentralForce(dir * -20000);
	}
		else 
	{
		enemy.getRigidbody()->translate(dir);
	}

	if ((node - enemy.getPositionBT()).length() < 0.8f)
		currentNode++;
}
