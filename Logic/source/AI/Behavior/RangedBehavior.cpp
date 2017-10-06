#include <AI\Behavior\RangedBehavior.h>
#include <AI\Enemy.h>
#include <Misc\RandomGenerator.h>

using namespace Logic;

RangedBehavior::RangedBehavior()
{
	m_distance = 20; // distance before stopping

	BehaviorNode &root = setRoot(NodeType::PRIORITY, 0, NULL);

	// WALK TOWARDS ?
	BehaviorNode &walkTowards = addNode(root, NodeType::CONDITION, 0,
		[](RunIn& in) -> bool {
			RangedBehavior *behavior = dynamic_cast<RangedBehavior*>(in.behavior);
			if ((in.enemy->getPosition() - in.target->getPosition()).Length()
				> behavior->getDistance())
				return true;
			else
				return false;
		}
	);

	// WALK TOWARDS & SHOOT !
	addNode(walkTowards, NodeType::ACTION, 0,
		[](RunIn& in) -> bool {
			RangedBehavior *behavior = dynamic_cast<RangedBehavior*>(in.behavior);
			if (RandomGenerator::singleton().getRandomInt(0, RangedBehavior::ABILITY_CHANCHE) == 0)
				in.enemy->useAbility(*in.target);
			behavior->walkTowardsPlayer(*in.enemy, *in.target, in.deltaTime);
			return true;
		}
	);

	// STAY ?
	BehaviorNode &stay = addNode(root, NodeType::RANDOM, 1, [](RunIn& in) -> bool {
		return true;
	});

	// SHOOT !
	addNode(stay, NodeType::ACTION, 995, [](RunIn& in) -> bool {
		if (RandomGenerator::singleton().getRandomInt(0,
			RangedBehavior::ABILITY_CHANCHE) == 0)

			in.enemy->useAbility(*in.target);
		return true;
	});

	// jump
	addNode(stay, NodeType::ACTION, 5, [](RunIn& in) -> bool {
		in.enemy->getRigidbody()->applyCentralForce({ 0, 1000, 0 });
		return true;
	});
}

int RangedBehavior::getDistance() const
{
	return m_distance;
}

void RangedBehavior::walkTowardsPlayer(Enemy &enemy, Player const &player, float deltaTime)
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

void RangedBehavior::update(Enemy &enemy, Player const &player, float deltaTime)
{
	// this is frame bound, fix it
	RunIn in { &enemy, &player, this, deltaTime };
	runTree(in);
}

void RangedBehavior::updatePath(Entity const &from, Entity const &to)
{
	m_path.loadPath(from, to);
	m_path.setCurrentNode(0);
}

void RangedBehavior::debugRendering(Graphics::Renderer &renderer)
{
}