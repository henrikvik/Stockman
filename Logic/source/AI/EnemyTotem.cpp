#include <AI\EnemyTotem.h>
using namespace Logic;

EnemyTotem::EnemyTotem(btRigidBody * body, btVector3 halfExtent)
    : Enemy(Resources::Models::Files::UnitCube, body, halfExtent, 0, 0, 0, EnemyType::UNNAMED_1, 0, btVector3(0, 0, 0))
{
}

EnemyTotem::~EnemyTotem()
{
}

void EnemyTotem::createAbilities()
{
}

void EnemyTotem::clear()
{
}

void EnemyTotem::onCollision(PhysicsObject & other, btVector3 contactPoint, float dmgMultiplier)
{
}

void EnemyTotem::updateSpecific(Player & player, float deltaTime)
{
}

void EnemyTotem::updateDead(float deltaTime)
{
}

void EnemyTotem::useAbility(Player & target)
{
}
