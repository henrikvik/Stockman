#include <AI\Behavior\RangedBehavior.h>
#include <AI\Enemy.h>
#include <Misc\RandomGenerator.h>

using namespace Logic;

RangedBehavior::RangedBehavior()
{
	m_distance = 20; // distance before stopping

	// create node test, Behavior Trees is a WIP, and will continue to evolve during sprint #3 & #4. ETA: Sprint #4
	BehaviorNode node;
}

void RangedBehavior::update(Enemy &enemy, Player const &player, float deltaTime)
{
	// this is frame bound, fix it
	if (RandomGenerator::singleton().getRandomInt(0, 100) == 0)
		enemy.useAbility(player);

	if ((enemy.getPosition() - player.getPosition()).Length() > m_distance)
	{
		btVector3 node = m_path.updateAndReturnCurrentNode(enemy, player);
		btVector3 dir = node - enemy.getPositionBT();

		dir = dir.normalize();
		dir *= deltaTime / 1000.f;
		dir *= 10;

		enemy.getRigidbody()->translate(dir);

		if ((node - enemy.getPositionBT()).length() < 0.3f)
			m_path.setCurrentNode(m_path.getCurrentNode() + 1);
	}
}

void RangedBehavior::updatePath(Entity const &from, Entity const &to)
{
	m_path.loadPath(from, to);
	m_path.setCurrentNode(0);
}

void RangedBehavior::debugRendering(Graphics::Renderer &renderer)
{
}