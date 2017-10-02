#include <AI\Behavior\TestBehavior.h> 
#include <AI\Enemy.h>

using namespace Logic;

TestBehavior::TestBehavior()
{
	
}

TestBehavior::~TestBehavior()
{
}

void TestBehavior::update(Enemy &enemy, Player const & player, float deltaTime)
{
	/* UNDER CONSTRUCTION FOR NOW
	btVector3 node = m_path.updateAndReturnCurrentNode(enemy, player);
	btVector3 dir = node - enemy.getPositionBT();

	dir = dir.normalize();
	dir *= deltaTime / 1000.f;
	dir *= enemy.getMoveSpeed();

	if (enemy.getHealth() < 5)
	{
		enemy.getRigidbody()->applyCentralForce(dir * -20000);
	}
		else 
	{
		enemy.getRigidbody()->translate(dir);
	}

	if ((node - enemy.getPositionBT()).length() < 0.8f)
		m_path.setCurrentNode(m_path.getCurrentNode() + 1);
		*/
}
