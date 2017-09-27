#ifndef TEST_BEHAVIOR_H
#define TEST_BEHAVIOR_H

#include "Behavior.h"

namespace Logic 
{
	class TestBehavior : public Behavior
	{
	public:
		virtual ~TestBehavior();

		virtual void update(Enemy &enemy, Player const &player, float deltaTime);
	};
}

#endif