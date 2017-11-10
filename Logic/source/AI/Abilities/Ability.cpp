#include <AI\Abilities\Ability.h>
using namespace Logic;

Ability::Ability(AbilityData const &data, std::function<void()> useFunction)
    : data(data), useFunction(useFunction) {

}

Ability::~Ability()
{
}

void Ability::update(float deltaTime)
{
}

bool Ability::useAbility(bool forceUse)
{
    if (forceUse || canUseAbility() && )
    {
        useFunction();
        return true;
    }
    else 
    {
        return false;
    }
}

bool Ability::canUseAbility() const
{
    return false;
}

const AbilityData &Ability::getData() const
{
    return data;
}
