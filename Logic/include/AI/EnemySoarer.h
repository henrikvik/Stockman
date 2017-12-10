#ifndef ENEMY_SOARER_H
#define ENEMY_SOARER_H

#include "Enemy.h"
#include "Abilities\Ability.h"

namespace Logic
{
    class EnemySoarer : public Enemy
    {
        private:
            static const int HEALTH, DAMAGE, SCORE;
            static const float SPEED, STEERING_MOD, AB1_SPEED,
                               HEIGHT_OFFSET, AB1_GRAVITY;
            static const btVector3 AB1_SCALE;

            float ab2Speed;
            btVector3 dir;

            Ability ab1, ab2;
            btVector3 gravity;
        public:
            EnemySoarer(btRigidBody* body, btVector3 halfExtent);
            virtual ~EnemySoarer();

            virtual void createAbilities();

            virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);

            virtual void updateSpecific(Player &player, float deltaTime);
            virtual void updateDead(float deltaTime);

            virtual void useAbility(Player &target);
            virtual void handleFlying(Player const &target);
    };
}

#endif