#ifndef TEST_BEHAVIOR_H
#define TEST_BEHAVIOR_H

#include "Behavior.h"

namespace Logic 
{
	class TestBehavior : public Behavior
	{
	private:
		std::vector<const DirectX::SimpleMath::Vector3*> path;
		int currentNode;
	public:
		TestBehavior();
		virtual ~TestBehavior();

		virtual void update(Enemy &enemy, Player const &player, float deltaTime);
	};
}

#endif