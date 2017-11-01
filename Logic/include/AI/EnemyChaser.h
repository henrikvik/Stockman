#ifndef ENEMY_CHASER
#define ENEMY_CHASER

#include "Enemy.h"

namespace Logic
{
    class EnemyChaser : public Enemy
    {
    private:
        static const float MAX_HP, BASE_DAMAGE, MOVE_SPEED;
    public:
        EnemyChaser(btRigidBody* body);
        ~EnemyChaser();

        virtual void updateDead(float deltaTime) {};
        virtual void updateSpecific(Player const &player, float deltaTime) {};
        virtual void onCollision(PhysicsObject& other, btVector3 contactPoint,
            float dmgMultiplier);
    };
}

#endif