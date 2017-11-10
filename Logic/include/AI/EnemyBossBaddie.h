#ifndef ENEMY_BOSS_BADDIE_H
#define ENEMY_BOSS_BADDIE_H

#include <AI\Enemy.h>
#include <btBulletCollisionCommon.h>

namespace Logic
{
    class EnemyBossBaddie : public Enemy
    {
    private:
        static const float BASE_SPEED, ABILITY_1_DURATION;
        static const int BASE_DAMAGE, MAX_HP, ABILITIES = 1;

        bool usingAbility[EnemyBossBaddie::ABILITIES];
        float abilityTimer[ABILITIES];

        void useAbility1(Entity &target);
    public:
        EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent);
        virtual void useAbility(Entity &target, int phase);

        virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
        virtual void updateSpecific(Player &player, float deltaTime);
        virtual void updateDead(float deltaTime);
    };
}

#endif