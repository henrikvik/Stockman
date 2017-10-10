#ifndef ENEMY_H
#define ENEMY_H

#include <Entity\Entity.h>
#include <Player\Player.h>
#include <AI\Behavior\Behavior.h>
#include <Projectile\ProjectileManager.h>

#pragma region Comment
/*
	CLASS: Enemy
	AUTHOR: Lukas Westling

	This class handles the Enemy and
	its flags is handled by EntityManager

	But wait? Why cant i check if an enemy is dead???
	That is because that is very slow and waste of cachelines!

	! changing this = bad performence !
*/
#pragma endregion ClassDesc

namespace Logic 
{
	class Enemy : public Entity 
	{
		private:
			float m_health, m_maxHealth, m_baseDamage, m_moveSpeed; // Base
			float m_moveSpeedMod;									// Variables for effect modifiers
			int m_enemyType;
			ProjectileManager *m_projectiles;
			// Animation m_animation;
		public:	
			enum BEHAVIOR_ID { TEST, RANGED };

			Enemy(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent, float maxHealth, float baseDamage, float moveSpeed, int enemyType, int animationId);
			virtual ~Enemy();

			void setProjectileManager(ProjectileManager *projectileManager);

			virtual void update(Player const &player, float deltaTime,
				std::vector<Enemy*> const &closeEnemies, bool updatePath = false);
			virtual void useAbility(Entity const &target) {};
			virtual void updateDead(float deltaTime) = 0;
			virtual void updateSpecific(Player const &player, float deltaTime) = 0;

			virtual void affect(int stacks, Effect const &effect, float dt);

			// for debugging
			void debugRendering(Graphics::Renderer &renderer);

			void damage(float damage);
			void setBehavior(BEHAVIOR_ID id);

			float getHealth() const;
			float getMaxHealth() const;
			float getBaseDamage() const;
			float getMoveSpeed() const;
			int getEnemyType() const;

			void spawnProjectile(btVector3 dir, Graphics::ModelID id, float speed);
			ProjectileManager* getProjectileManager() const;
		protected: //for testing
			Logic::Behavior *m_behavior;
			//btRigidBody *weakPoint;
	};
}

#endif