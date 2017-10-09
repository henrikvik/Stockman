#include <AI\Behavior\Behavior.h>
#include <AI\Enemy.h>
#include <Misc\RandomGenerator.h>

#include <queue>

using namespace Logic;

Behavior::~Behavior()
{
}

void Behavior::walkPath(SimplePathing pathing, RunIn &in)
{
	btVector3 node = pathing.updateAndReturnCurrentNode(*in.enemy, *in.target);
	btVector3 dir = node - in.enemy->getPositionBT();

	dir = dir.normalize();
	dir *= in.deltaTime / 1000.f;
	dir *= 10;

	in.enemy->getRigidBody()->translate(dir);

	if ((node - in.enemy->getPositionBT()).length() < 0.3f)
		pathing.setCurrentNode(pathing.getCurrentNode() + 1);
}

void Behavior::runTree(RunIn &in)
{
	runNode(in, m_root);
}

void Behavior::setRoot(NodeType type, int value, run func)
{
	m_root = { type, value, {}, func };
}

Behavior::BehaviorNode* Behavior::addNode(BehaviorNode *parent,
	NodeType type, int value, run func)
{
	int index = parent->children.size();
	parent->children.push_back({ type, value, {}, func });

	if (parent->type == PRIORITY)
	{
		while (index > 0 &&
			parent->children[index].value > parent->children[index - 1].value)
		{
			std::swap(parent->children[index], parent->children[index - 1]);
			index--; // weird..
		}
	}

	return &parent->children[index];
}

bool Behavior::runNode(RunIn &in, BehaviorNode &node)
{
	// Recursive method to run the entire tree, todo: opt
	int totalWeight = 0, curr = 0, rng;
	switch (node.type)
	{
		case NodeType::ACTION: // Action, just run the node
			return node.run(in);
			break;
		case NodeType::RANDOM: // Random selector, run a random node
			for (BehaviorNode &n : node.children)
				totalWeight += n.value;
			rng = RandomGenerator::singleton().getRandomInt(0, totalWeight);
			for (BehaviorNode &n : node.children)
			{
				curr += n.value;
				if (rng <= curr)
					return runNode(in, n);
			}
			break;
		case NodeType::CONDITION: // Run the node, if true run children in seq
			if (node.run(in))
				for (BehaviorNode &n : node.children)
					runNode(in, n);
			else
				return false;
			break;
		case NodeType::PRIORITY: // run the most priority node, if it failed run next most value etc etc
			// this is already sorted
			for (BehaviorNode &n : node.children)
				if (runNode(in, n))
					return true;
			break;
		case NodeType::SEQUENCE: // run all the nodes
			for (BehaviorNode &n : node.children)
				runNode(in, n);
			break;
	}
	return true;
}

Behavior::BehaviorNode* Behavior::getRoot() {
	return &m_root; 
}
