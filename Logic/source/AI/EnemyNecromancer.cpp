#include <AI\EnemyNecromancer.h>
#include <AI\Behavior\RangedBehavior.h>
#include <Misc\RandomGenerator.h>
#include <Projectile\Projectile.h>
#include <Misc\ComboMachine.h>

using namespace Logic;

const int EnemyNecromancer::SPEED_AB1 = 15,
          EnemyNecromancer::SPEED_AB2 = 25,
          EnemyNecromancer::MAX_SPAWNED_MINIONS = 4,
          EnemyNecromancer::BASE_DAMAGE = 1,
          EnemyNecromancer::MAX_HP = 50;
const float EnemyNecromancer::BASE_SPEED = 8.f;

EnemyNecromancer::EnemyNecromancer(btRigidBody* body, btVector3 halfExtent)
    : Enemy(Resources::Models::UnitCube, body, halfExtent, MAX_HP, BASE_DAMAGE,
        BASE_SPEED, EnemyType::NECROMANCER, 0) {
    setBehavior(RANGED);
    addCallback(ON_DEATH, [&](CallbackData data) -> void {
        ComboMachine::Get().Kill(getEnemyType());
        SpawnTrigger(2, getPositionBT(), std::vector<int>{ StatusManager::AMMO_PICK_UP_PRIMARY });
    });
    m_spawnedMinions = 0;
}

EnemyNecromancer::~EnemyNecromancer()
{
}

void EnemyNecromancer::clear()
{
}

void EnemyNecromancer::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    if (Enemy *e = dynamic_cast<Enemy*>(&other))
    {
    }
    else if (Projectile *pj = dynamic_cast<Projectile*> (&other))
    {
        if (!pj->getProjectileData().enemyBullet)
        {
            damage(static_cast<int> (pj->getProjectileData().damage * dmgMultiplier));

            if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
        }
    }
}

void EnemyNecromancer::onCollision(Player &other)
{

}

void EnemyNecromancer::updateSpecific(Player &player, float deltaTime)
{
}

void EnemyNecromancer::updateDead(float deltaTime)
{
    Entity::update(deltaTime);
}

void EnemyNecromancer::useAbility(Entity &target)
{
    if (RandomGenerator::singleton().getRandomInt(0, 800))
    {
        if (m_spawnedMinions < MAX_SPAWNED_MINIONS)
        {
            Projectile *pj = shoot(((target.getPositionBT() - getPositionBT()) + btVector3{ 0, 80, 0 }).normalize(), Resources::Models::UnitCube, (float)SPEED_AB2, 2.5f, 0.6f);
            pj->addCallback(ON_COLLISION, [&](CallbackData &data) -> void {
                if (m_spawnedMinions < MAX_SPAWNED_MINIONS)
                {
                    Enemy *e = SpawnEnemy(EnemyType::NECROMANCER_MINION, data.caller->getPositionBT(), {});
                    m_spawnedMinions++;

                    increaseCallbackEntities();
                    e->addCallback(ON_DEATH, [&](CallbackData &data) -> void {
                        m_spawnedMinions--;
                    });
                    e->addCallback(ON_DESTROY, [&](CallbackData data) -> void {
                        decreaseCallbackEntities();
                    });
                }
            });
        }
        else
        {
            shoot((target.getPositionBT() - getPositionBT()).normalize(), Resources::Models::UnitCube, (float)SPEED_AB1, 1.1f, 0.2f);
        }
    }
}