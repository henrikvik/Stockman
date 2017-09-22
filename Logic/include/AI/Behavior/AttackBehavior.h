#ifndef ATTACK_BEHAVIOR_H
#define ATTACK_BEHAVIOR_H

#include "Behavior.h"

namespace Logic 
{
	class Enemy;

	class AttackBehavior : public Behavior
	{
	private:
		Enemy *m_toUpdate;
	public:
		AttackBehavior(Enemy *toUpdate);
		virtual ~AttackBehavior();

		virtual void update(Player const &player, float deltaTime);
	};
}

#endif