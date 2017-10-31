#include <AI\Behavior\MeleeBehavior.h> 
using namespace Logic;

MeleeBehavior::MeleeBehavior() : Behavior(PathingType::CHASING)
{
    setRoot(NodeType::PRIORITY, 0, NULL);
    addNode(getRoot(), NodeType::ACTION, 0, [](RunIn &in) -> bool {
        in.behavior->walkPath(in);
        return true;
    });
}

MeleeBehavior::~MeleeBehavior()
{
}