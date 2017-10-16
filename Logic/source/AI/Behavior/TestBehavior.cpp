#include <AI\Behavior\TestBehavior.h> 
#include <AI\Enemy.h>

using namespace Logic;

TestBehavior::TestBehavior() : Behavior(PathingType::CHASING)
{
	setRoot(NodeType::ACTION, 0, [](RunIn &in) -> bool {
		in.behavior->walkPath(in);
	});
}