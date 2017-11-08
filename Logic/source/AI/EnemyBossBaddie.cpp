#include <AI\EnemyBossBaddie.h>
#include <Graphics\include\Structs.h>
using namespace Logic;

const float EnemyBossBaddie::BASE_SPEED = 1.5f;
const int EnemyBossBaddie::BASE_DAMAGE = 1, EnemyBossBaddie::MAX_HP = 1000; // ye he is though

EnemyBossBaddie::EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent)
    : Enemy(Graphics::ModelID::ENEMYGRUNT, body,
    halfExtent, MAX_HP, BASE_DAMAGE, BASE_SPEED, EnemyType::BOSS_1, 0)
{
    setBehavior(BOSS_BADDIE);
}

void EnemyBossBaddie::useAbility(Entity const &target, int phase)
{
}

void EnemyBossBaddie::onCollision(PhysicsObject &other, btVector3 contactPoint, float dmgMultiplier)
{
}

void EnemyBossBaddie::updateSpecific(Player const &player, float deltaTime)
{
}

void EnemyBossBaddie::updateDead(float deltaTime)
{
}