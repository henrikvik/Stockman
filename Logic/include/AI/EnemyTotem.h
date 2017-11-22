#ifndef ENEMY_TOTEM_H
#define ENEMY_TOTEM_H

#include "Enemy.h"
#include "Abilities\Ability.h"

namespace Logic
{
    class EnemyTotem : public Enemy
    {
    private:
        Ability shoot;

        int m_spawnedMinions;
        static const float BASE_SPEED;
        static const int BASE_DAMAGE, MAX_HP, SCORE;
        static const int SPEED_SHOOT, MAX_SPAWNED_MINIONS;
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