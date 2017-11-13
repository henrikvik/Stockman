#ifndef ENEMY_NECROMANCER_H
#define ENEMY_NECROMANCER_H

#include <AI\Enemy.h>
#include <Player\Player.h>

namespace Logic
{
	class EnemyNecromancer : public Enemy
	{
		private:
            int m_spawnedMinions;
            static const float BASE_SPEED;
            static const int BASE_DAMAGE, MAX_HP;
			static const int SPEED_AB1, SPEED_AB2, MAX_SPAWNED_MINIONS;
		public:
			EnemyNecromancer(btRigidBody* body, btVector3 halfExtent);
			virtual ~EnemyNecromancer();

			virtual void clear();
      
			virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
			virtual void onCollision(Player& other);

			virtual void updateSpecific(Player &player, float deltaTime);
			virtual void updateDead(float deltaTime);

			virtual void useAbility(Player &target);
	};
}

#endif