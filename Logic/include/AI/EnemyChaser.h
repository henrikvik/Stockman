#ifndef ENEMY_CHASER
#define ENEMY_CHASER

#include "Enemy.h"

namespace Logic
{
    class EnemyChaser : public Enemy
    {
    private:
        static const int MAX_HP, BASE_DAMAGE;
        static const float MOVE_SPEED;
        AnimatedRenderInfo animatedRenderInfo;
    public:
        EnemyChaser(btRigidBody* body);
        ~EnemyChaser();

        // replace later
        void loadAnimation(Resources::Models::Files model);

        virtual void updateDead(float deltaTime) {};
        virtual void updateSpecific(Player &player, float deltaTime);
        virtual void onCollision(PhysicsObject& other, btVector3 contactPoint,
            float dmgMultiplier);

        virtual void renderSpecific() const;
    };
}

#endif