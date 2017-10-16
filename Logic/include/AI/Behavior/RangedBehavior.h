#ifndef RANGED_BEHAVIOR_H
#define RANGED_BEHAVIOR_H

#include "Behavior.h" 

namespace Logic
{
	class RangedBehavior : public Behavior
	{
	private:
		float m_distance;

		static const int ABILITY_CHANCHE = 150;
	public:
		RangedBehavior();
		virtual ~RangedBehavior() {}

		int getDistance() const;

		virtual void updateSpecific(Enemy &enemy, std::vector<Enemy*> const &closeEnemies,
			Player const &player, float deltaTime);
	};
}

#endif