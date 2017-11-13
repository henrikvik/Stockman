#ifndef ABILITY_H
#define ABILITY_H

#include <Player\Player.h>
#include <functional>

namespace Logic
{
    struct AbilityData
    {
        float duration, cooldown;
        // random from 0 to randomChanche, if result is 0, do ability
        int randomChanche;
    };

    class Ability
    {
        private:
            static const float GCD;
            bool usingAbility;
            float currentCooldown, currentDuration;

            std::function<void(Player &player, Ability &ability)> onUse, onTick;
            AbilityData data;
        public:
            Ability(); // dont use
            Ability(AbilityData const &data, std::function<void(Player&, Ability&)> onTick,
                std::function<void(Player&, Ability &ability)> onUse);
            virtual ~Ability();

            // reduces cd etc
            void update(float deltaTime, Player &player);
            // returns true if ability is used, if forceUse is true it skips cooldown checking
            bool useAbility(Player &player, bool forceUse = false);
            bool canUseAbility() const;

            float getCurrentCooldown() const;
            float getCurrentDuration() const;
            bool isUsingAbility() const;

            const AbilityData& getData() const;
    };
}

#endif