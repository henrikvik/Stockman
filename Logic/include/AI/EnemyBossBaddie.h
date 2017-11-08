#ifndef ENEMY_BOSS_BADDIE_H
#define ENEMY_BOSS_BADDIE_H

#include <AI\Enemy.h>
#include <btBulletCollisionCommon.h>

namespace Logic
{
    class EnemyBossBaddie : public Enemy
    {
    private:
        static const float BASE_SPEED;
        static const int BASE_DAMAGE, MAX_HP;
    public:
        EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent);
        virtual void useAbility(Entity const &target, int phase);

        virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
        virtual void updateSpecific(Player const &player, float deltaTime);
        virtual void updateDead(float deltaTime);
    };
}

#endif