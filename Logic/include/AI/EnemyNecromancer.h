#ifndef ENEMY_NECROMANCER_H
#define ENEMY_NECROMANCER_H

#include <AI\Enemy.h>
#include <Player\Player.h>

namespace Logic
{
	class EnemyNecromancer : public Enemy
	{
		public:
			EnemyNecromancer(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent);
			virtual ~EnemyNecromancer();

			virtual void clear();
			virtual void onCollision(Entity& other);
			virtual void onCollision(Player& other);
			virtual void updateSpecific(Player const &player, float deltaTime);
			virtual void updateDead(float deltaTime);

			virtual void useAbility(Entity const &target);
	};
}

#endif