#include <AI\Behavior\RangedBehavior.h>
#include <AI\Enemy.h>
#include <Misc\RandomGenerator.h>

using namespace Logic;
const float RangedBehavior::FLEE_MOD = 1.33f;

RangedBehavior::RangedBehavior() :
	Behavior(PathingType::CHASING)
{
	m_distance = 20; // distance before stopping
	setRoot(NodeType::PRIORITY, 0, NULL);

	/*
		IMP: WHEN USING NODETYPE:PROIRITY, ADDING CHILDREN SHOULD BE DONE IMMEDIATELY
		FOR PERF AND NOT CRASHING

		Should change behvaior trees to follow Data Oriented Model, but for now
		boids is more important. But the infractstructure is here.
	*/

	// WALK TOWARDS ?
	BehaviorNode *walkTowards = addNode(getRoot(), NodeType::CONDITION, 1,
		[](RunIn& in) -> bool {
			RangedBehavior *behavior = dynamic_cast<RangedBehavior*>(in.behavior);
			return (in.enemy->getPosition() - in.target->getPosition()).Length()
					> behavior->getDistance();
		}
	);

	// WALK TOWARDS & SHOOT !
	addNode(walkTowards, NodeType::ACTION, 1,
		[](RunIn& in) -> bool {
			RangedBehavior *behavior = dynamic_cast<RangedBehavior*>(in.behavior);

            in.enemy->useAbility(*in.target);
			behavior->walkPath(in);

			return true;
		}
	);

	// omg 
	BehaviorNode *flee = addNode(getRoot(), NodeType::CONDITION, 2,
		[](RunIn& in) -> bool {
            RangedBehavior *behavior = dynamic_cast<RangedBehavior*>(in.behavior);
			return (in.enemy->getPosition() - in.target->getPosition()).Length()
				< behavior->getDistance();
		}
	);

	addNode(flee, NodeType::ACTION, 0, [](RunIn& in) -> bool {
            in.enemy->useAbility(*in.target);
            btVector3 dir = (in.enemy->getPositionBT() - in.target->getPositionBT());
            dir.setY(0);
            in.enemy->getRigidBody()->setLinearVelocity(dir.normalized() * in.enemy->getMoveSpeed() * FLEE_MOD);
			return true;
		}
	);
}

float RangedBehavior::getDistance() const
{
	return m_distance;
}

void RangedBehavior::updateSpecific(Enemy &enemy, std::vector<Enemy*> const &closeEnemies,
	Player const &player, float deltaTime)
{
}