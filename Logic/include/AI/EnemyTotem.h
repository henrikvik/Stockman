#ifndef ENEMY_TOTEM_H
#define ENEMY_TOTEM_H

#include "Enemy.h"
#include "Abilities\Ability.h"

namespace Logic
{
    class EnemyTotem : public Enemy
    {
    private:
        Ability spreadShot;

        static const float BASE_SPEED, BULLET_SPEED;
        static const float AB_ROTATION;
        static const int BASE_DAMAGE, MAX_HP, SCORE;
        static const int BULLET_AMOUNT;
        static const btVector3 AB_SCALE;

        float m_rotation;
    public:
        EnemyTotem(btRigidBody* body, btVector3 halfExtent);
        virtual ~EnemyTotem();

        virtual void createAbilities();
        virtual void clear();

        virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);

        virtual void updateSpecific(Player &player, float deltaTime);
        virtual void updateDead(float deltaTime);

        virtual void useAbility(Player &target);
    };
}

#endif