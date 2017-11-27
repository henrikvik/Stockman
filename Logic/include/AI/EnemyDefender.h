#ifndef ENEMY_DEFENDER_H
#define ENEMY_DEFENDER_H

#include "Enemy.h"
#include "Abilities\Ability.h"

namespace Logic
{
    class EnemyDefender : public Enemy
    {
    private:
        Ability m_melee;
        std::vector<Projectile*> m_projectiles;

        int m_spawnedMinions;
        static const float BASE_SPEED;
        static const int BASE_DAMAGE, MAX_HP, SCORE, PROJECTILES;
        static const float MELEE_DISTANCE, PROJECTILE_SPEED;
    public:
        EnemyDefender(btRigidBody* body, btVector3 halfExtent);
        virtual ~EnemyDefender();

        virtual void onSpawn();

        virtual void createAbilities();
        virtual void clear();

        virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);

        virtual void updateSpecific(Player &player, float deltaTime);
        virtual void updateDead(float deltaTime);

        virtual void useAbility(Player &target);
    };
}

#endif