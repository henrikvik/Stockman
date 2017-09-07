#ifndef ENEMY_H
#define ENEMY_H

#include <Entity/Entity.h>

#pragma region Comment
/*
	CLASS: Enemy
	AUTHOR: Lukas Westling

	This class handles the Enemy and
	its flags is in EntityManager

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
			float m_health, m_baseDamage; 
			int m_enemyType;
			// Animation m_animation;
		public:	
			Enemy(float health, float baseDamage, int enemyType, int animationId);
			virtual ~Enemy();

			void update(float deltaTime);
			virtual void updateSpec(float deltaTime) = 0;
	};
}

#endif