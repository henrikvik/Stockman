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
            ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, MELEE // TODO: give real names
        };
        std::unordered_map<AbilityId, Ability> abilities;

        static const float BASE_SPEED, PROJECTILE_SPEED, ABILITY_1_MOD, MELEE_RANGE, MELEE_PUSHBACK;
        static const int BASE_DAMAGE, MAX_HP;

        std::vector<TextRenderInfo> info;
        std::vector<std::wstring> infoText;

        float testTime;
        float ability3Offset = 0;
    public:
        EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent);

        void createAbilities();
        virtual void useAbility(Player &target);
        virtual void useAbility(Player &target, int phase);

        virtual void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
        virtual void updateSpecific(Player &player, float deltaTime);
        virtual void updateDead(float deltaTime);

        virtual void renderSpecific() const;
    };
}

#endif