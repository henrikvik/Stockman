#ifndef ENEMY_H
#define ENEMY_H

#include <Graphics\include\RenderQueue.h>

#include <Entity\Entity.h>
#include <AI\EnemyType.h>

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
    class Player;
    class Behavior;

	class Enemy : public Entity
	{
		private:
            // This is used to count how many callbacks is added, 
            // so this HAS to be ZERO before this is destroyed or
            // the showcase at PAX East will go bad
            int m_nrOfCallbacksEntities;

            // base
            int m_health, m_maxHealth, m_baseDamage;
            float m_moveSpeed;

			float m_bulletTimeMod;									// Variables for effect modifiers
            float m_moveSpeedMod;
			EnemyType m_enemyType;

			Behavior *m_behavior;

            StaticRenderInfo enemyRenderInfo;
            LightRenderInfo light;

            //status
            bool m_stunned;
            float m_fireTimer;
		public:	
			enum BEHAVIOR_ID { TEST, RANGED, MELEE, BOSS_BADDIE };

			Enemy(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent, int maxHealth, int baseDamage, float moveSpeed, EnemyType enemyType, int animationId, btVector3 modelOffset = { 0.f, 0.f, 0.f });
			virtual ~Enemy();

			virtual void update(Player &player, float deltaTime,
				std::vector<Enemy*> const &closeEnemies);

            virtual void useAbility(Player &target) {};
            virtual void useAbility(Player &target, int phase) { useAbility(target); };

			virtual void updateDead(float deltaTime) = 0;
			virtual void updateSpecific(Player &player, float deltaTime) = 0;

			virtual void affect(int stacks, Effect const &effect, float dt);
            void onEffectEnd(int stacks, Effect const &effect);

			Projectile* shoot(btVector3 dir, Resources::Models::Files id, float speed, float gravity, float scale, bool sensor = false);

			// for debugging
			void debugRendering();

            void increaseCallbackEntities();
            void decreaseCallbackEntities();
            bool hasCallbackEntities();

			virtual void damage(int damage);
			void setBehavior(BEHAVIOR_ID id);
			void setEnemyType(EnemyType id);

            int getHealth() const;
            int getMaxHealth() const;
            int getBaseDamage() const;

			float getMoveSpeed() const;
            EnemyType getEnemyType() const;
			Behavior* getBehavior() const;

            void render() const;
            virtual void renderSpecific() const {};
	};
}

#endif