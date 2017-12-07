#include <AI\Behavior\MeleeBehavior.h>
#include <AI\Enemy.h>
namespace Logic
{
    MeleeBehavior::MeleeBehavior() : Behavior(PathingType::CHASING)
    {
        setRoot(NodeType::PRIORITY, 0, NULL);
        addNode(getRoot(), NodeType::ACTION, 0, [](RunIn &in) -> bool
        {
            in.behavior->walkPath(in);
            in.enemy->useAbility(*in.target);
            return true;
        });
    }

    MeleeBehavior::~MeleeBehavior()
    {
    }
}
