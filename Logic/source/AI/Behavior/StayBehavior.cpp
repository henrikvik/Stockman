#include <AI\Behavior\StayBehavior.h>
#include <AI\Enemy.h>
using namespace Logic;

StayBehavior::StayBehavior() 
    : Behavior(PathingType::NO_PATHING)
{
    setRoot(NodeType::ACTION, 0, [](RunIn &in) -> bool {
        in.enemy->useAbility(*in.target);
        return true;
    });
}

StayBehavior::~StayBehavior()
{
}
