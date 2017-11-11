#include <AI\Abilities\Ability.h>
#include <Misc\RandomGenerator.h>
using namespace Logic;

const float Ability::GCD = 100.f;

Ability::Ability()
{
    // dont use, only for init in a map
}

Ability::Ability(AbilityData const &data,
    std::function<void(Player&, Ability&)> onTick,
    std::function<void(Player&, Ability&)> onUse)
    : data(data), onUse(onUse), onTick(onTick) {
    usingAbility = false;
    currentDuration = 0.f;
    currentCooldown = 0.f;
}

Ability::~Ability()
{
}

void Ability::update(float deltaTime, Player &player)
{
    currentCooldown -= deltaTime; // it keeps counting down, should realistically never get a int underflow
    if (usingAbility)
    {
        currentDuration -= deltaTime;
        onTick(player, *this);
        if (currentDuration <= 0)
        {
            usingAbility = false;
            currentDuration = 0.f;
        }
    }
}

bool Ability::useAbility(Player &player, bool forceUse)
{

    if (forceUse)
    {
        onUse(player, *this);
        return true;
    }

    if (canUseAbility())
    {
        if (RandomGenerator::singleton().getRandomInt(0, data.randomChanche) == 0)
        {
            currentCooldown = data.cooldown;
            currentDuration = data.duration;
            usingAbility = true;

            onUse(player, *this);
            return true;
        }
        else
        {
            currentCooldown = GCD;
        }
    }

    return false;
}

bool Ability::canUseAbility() const
{
    return currentCooldown <= 0.f && !isUsingAbility();
}

float Ability::getCurrentCooldown() const
{
    return currentCooldown;
}

float Ability::getCurrentDuration() const
{
    return currentDuration;
}

bool Ability::isUsingAbility() const
{
    return usingAbility;
}

const AbilityData& Ability::getData() const
{
    return data;
}
