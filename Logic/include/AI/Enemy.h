#ifndef ENEMY_H
#define ENEMY_H

#include <Graphics\include\RenderQueue.h>

#include <Entity\Entity.h>
#include <Entity\AnimatedModel.h>
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
            static const int MIN_Y;
            static const float MAX_TARGET_Y;
            // This is used to count how many callbacks is added, 
            // so this HAS to be ZERO before this is destroyed or
            // the showcase at PAX East will go PAX South
            int m_nrOfCallbacksEntities;

            // base
            int m_health, m_maxHealth, m_baseDamage;
            float m_moveSpeed;

			float m_bulletTimeMod;									// Variables for effect modifiers
            float m_moveSpeedMod;
			EnemyType m_enemyType;

			Behavior *m_behavior;

            AnimatedModel animatedModel;
            float maxAnimationTime;

            bool m_stunned;
            float m_fireTimer;

            float m_blinkTimer;
        protected:
            LightRenderInfo light;
		public:	
			enum BEHAVIOR_ID { TEST, RANGED, MELEE, BOSS_BADDIE, STAY };

			Enemy(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent, int maxHealth, int baseDamage, float moveSpeed, EnemyType enemyType, int animationId, btVector3 modelOffset = { 0.f, 0.f, 0.f });
			virtual ~Enemy();

			virtual void update(Player &player, float deltaTime,
				std::vector<Enemy*> const &closeEnemies);
            virtual void updateAnimation(float deltaTime);

            virtual void useAbility(Player &target) {};
            virtual void useAbility(Player &target, int phase) { useAbility(target); };

			virtual void updateDead(float deltaTime) = 0;
			virtual void updateSpecific(Player &player, float deltaTime) = 0;

            virtual void onSpawn() {};

			virtual void affect(int stacks, Effect const &effect, float dt);
            void onEffectEnd(int stacks, Effect const &effect);

            Projectile* shoot(btVector3 dir, Resources::Models::Files id, float speed, float gravity, btVector3 scale, bool sensor = false);
            Projectile* shoot(btVector3 dir, ProjectileData data, float speed, float gravity, btVector3 scale, bool sensor = false);

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
            float getSpeedMod() const;

            EnemyType getEnemyType() const;
			Behavior* getBehavior() const;

            AnimatedModel& getAnimatedModel();

            void render() const;
            virtual void renderSpecific() const {};
	};
}

#endif