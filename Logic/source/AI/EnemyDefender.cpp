#include <AI\EnemyDefender.h>
#include <Misc\ComboMachine.h>
#include <Projectile\Projectile.h>
using namespace Logic;

const float EnemyDefender::BASE_SPEED = 5.f;
const float EnemyDefender::MELEE_DISTANCE = 5.f;

const int   EnemyDefender::BASE_DAMAGE = 1,
            EnemyDefender::MAX_HP = 50,
            EnemyDefender::SCORE = 75,
            EnemyDefender::PROJECTILES = 20;

EnemyDefender::EnemyDefender(btRigidBody *body, btVector3 halfExtent)
    : Enemy(Resources::Models::Barrel, body, halfExtent, MAX_HP,
        BASE_DAMAGE, BASE_SPEED, EnemyType::BULLET_SPONGE, 0, btVector3()) {
    setBehavior(MELEE);
    init();
}

void EnemyDefender::init()
{
    m_projectiles.reserve(PROJECTILES);
    addCallback(ON_DEATH, [&](CallbackData data) -> void {
        ComboMachine::Get().kill(SCORE);
        RandomGenerator::singleton().getRandomInt(0, 1) ?
            SpawnTrigger(2, getPositionBT() - btVector3(0.f, data.caller->getRigidBody()->getCollisionShape()->getLocalScaling().y() * 1.5f, 0.f), std::vector<int>{ StatusManager::AMMO_PICK_UP_PRIMARY }) :
            SpawnTrigger(3, getPositionBT() - btVector3(0.f, data.caller->getRigidBody()->getCollisionShape()->getLocalScaling().y() * 1.5f, 0.f), std::vector<int>{ StatusManager::AMMO_PICK_UP_SECONDARY });
    });

    ProjectileData pdata;
    RandomGenerator &rng = RandomGenerator::singleton();

    pdata.damage = 0;
    pdata.enemyBullet = true;
    pdata.ttl = 999999; // is destroyed manually

    Projectile *pj;

    for (int i = 0; i < PROJECTILES; i++)
    {
        pj = SpawnProjectile(pdata, getPositionBT() +
            btVector3(rng.getRandomFloat(-5.f, 5.f), 0.f, 0.f), btVector3(), *this);
        pj->addCallback(ON_COLLISION, [](CallbackData &data) -> void {
            dynamic_cast<Projectile*> (data.caller)->setDead(true);
        });
        m_projectiles.push_back(pj);
    }
}

EnemyDefender::~EnemyDefender()
{
}

void EnemyDefender::createAbilities()
{
}

void EnemyDefender::clear()
{
}

void EnemyDefender::onCollision(PhysicsObject & other, btVector3 contactPoint, float dmgMultiplier)
{
}

void EnemyDefender::onCollision(Player & other)
{
}

void EnemyDefender::updateSpecific(Player & player, float deltaTime)
{
}

void EnemyDefender::updateDead(float deltaTime)
{
}

void EnemyDefender::useAbility(Player & target)
{
}
