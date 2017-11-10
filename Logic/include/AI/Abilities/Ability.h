#ifndef ABILITY_H
#define ABILITY_H

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
            bool isUsingAbility;
            std::function<void()> useFunction;
            AbilityData data;
        public:
            Ability(AbilityData const &data, std::function<void()> useFunction);
            virtual ~Ability();

            // reduces cd etc
            void update(float deltaTime);
            // returns true if ability is used, if forceUse is true it skips cooldown checking
            bool useAbility(bool forceUse);
            bool canUseAbility() const;

            const AbilityData& getData() const;
    };
}

#endif