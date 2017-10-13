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

		static const int ABILITY_CHANCHE = 150;
	public:
		RangedBehavior();
		virtual ~RangedBehavior() {}

		int getDistance() const;

		void walkTowardsPlayer(Enemy &enemy, Player const &player, float deltaTime);

		virtual void update(Enemy &enemy, std::vector<Enemy*> const &closeEnemies,
			Player const &player, float deltaTime);
		virtual void updatePath(Entity const &from, Entity const &to);
		virtual void debugRendering(Graphics::Renderer &renderer);

		SimplePathing getPath() const;
	};
}

#endif