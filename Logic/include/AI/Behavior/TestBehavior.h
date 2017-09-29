#ifndef TEST_BEHAVIOR_H
#define TEST_BEHAVIOR_H

#include "Behavior.h" 

namespace Logic 
{
	class TestBehavior : public Behavior
	{
	private:
		SimplePathing m_path;
	public:
		TestBehavior();
		virtual ~TestBehavior();

		virtual void update(Enemy &enemy, Player const &player, float deltaTime);
	};
}

#endif