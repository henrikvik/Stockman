#ifndef RANGED_BEHAVIOR_H
#define RANGED_BEHAVIOR_H

#include "Behavior.h" 

namespace Logic
{
	class RangedBehavior : public Behavior
	{
	private:
		SimplePathing m_path;
		float m_distance;

	public:
		RangedBehavior();
		virtual ~RangedBehavior() {}

		int getDistance() const;

		void walkTowardsPlayer(Enemy &enemy, Player const &player, float deltaTime);

		virtual void update(Enemy &enemy, Player const &player, float deltaTime);
		virtual void updatePath(Entity const &from, Entity const &to);
		virtual void debugRendering(Graphics::Renderer &renderer);

		static const int ABILITY_CHANCHE = 150;
	};
}

#endif