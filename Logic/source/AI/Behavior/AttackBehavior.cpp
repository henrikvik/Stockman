#include <AI\Behavior\AttackBehavior.h>
using namespace Logic;

AttackBehavior::AttackBehavior(Enemy *toUpdate)
{
	m_toUpdate = toUpdate;
}

AttackBehavior::~AttackBehavior()
{
}

void AttackBehavior::update(Player const & player, float deltaTime)
{

}
