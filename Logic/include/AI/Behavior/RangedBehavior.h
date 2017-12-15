#ifndef RANGED_BEHAVIOR_H
#define RANGED_BEHAVIOR_H

#include "Behavior.h" 

namespace Logic
{
	class RangedBehavior : public Behavior
	{
	private:
		float m_distance;
        static const float FLEE_MOD;
	public:
		RangedBehavior();
		virtual ~RangedBehavior() {}

		float getDistance() const;

		virtual void updateSpecific(Enemy &enemy, std::vector<Enemy*> const &closeEnemies,
			Player const &player, float deltaTime);
	};
}

#endif