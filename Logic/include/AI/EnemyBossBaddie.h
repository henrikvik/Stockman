#ifndef ENEMY_BOSS_BADDIE_H
#define ENEMY_BOSS_BADDIE_H

#include <AI\Enemy.h>
#include <unordered_map>
#include <string>
#include <AI\Abilities\Ability.h>
#include <btBulletCollisionCommon.h>

namespace Logic
{
    class EnemyBossBaddie : public Enemy
    {
    private:
        enum class AbilityId
        {
            ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE // TODO: give real names
        };
        std::unordered_map<AbilityId, Ability> abilities;

        static const float BASE_SPEED, PROJECTILE_SPEED, ABILITY_1_MOD;
        static const int BASE_DAMAGE, MAX_HP;

        std::vector<TextRenderInfo> info;
        std::vector<std::wstring> infoText;
        float testTime;
    public:
        EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent);

        void createAbilities();
        virtual void useAbility(Player &target, int phase);

        virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
        virtual void updateSpecific(Player &player, float deltaTime);
        virtual void updateDead(float deltaTime);
    };
}

#endif