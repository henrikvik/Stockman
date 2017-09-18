#ifndef ENEMY_H
#define ENEMY_H

#include <Entity/Entity.h>

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
			float m_health, m_maxHealth, m_baseDamage; 
			int m_enemyType;
			// Animation m_animation;
		public:	
			Enemy(btRigidBody* body, float maxHealth, float baseDamage, int enemyType, int animationId);
			virtual ~Enemy();

			virtual void update(float deltaTime);
			virtual void updateDead(float deltaTime) = 0;
			virtual void updateSpecific(float deltaTime) = 0;

			void damage(float damage);

			float getHealth() const;
			float getMaxHealth() const;
			float getBaseDamage() const;
			int getEnemyType() const;
	};
}

#endif