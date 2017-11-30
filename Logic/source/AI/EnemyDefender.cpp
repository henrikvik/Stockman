#include <AI\EnemyDefender.h>
#include <Misc\ComboMachine.h>
#include <Projectile\Projectile.h>
using namespace Logic;

const float EnemyDefender::BASE_SPEED = 14.5f;
const float EnemyDefender::MELEE_DISTANCE = 35.f,
            EnemyDefender::PROJECTILE_SPEED = 115.f;

const int   EnemyDefender::BASE_DAMAGE = 1,
            EnemyDefender::MAX_HP = 100,
            EnemyDefender::SCORE = 100,
            EnemyDefender::PROJECTILES = 22;

EnemyDefender::EnemyDefender(btRigidBody *body, btVector3 halfExtent)
    : Enemy(Resources::Models::UnitCube, body, halfExtent, MAX_HP,
        BASE_DAMAGE, BASE_SPEED, EnemyType::DEFENDER, 0) {
    setBehavior(MELEE);

    m_defenseTime = 0.f;

    light.color = DirectX::SimpleMath::Color(0.7f, 0.0f, 1.0f);
    light.intensity = 1.8f;
    light.range = 20.0f;
}

EnemyDefender::~EnemyDefender()
{
}

void EnemyDefender::onSpawn()
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
    pdata.meshID = Resources::Models::Ammocrystal;
    pdata.speed = PROJECTILE_SPEED;
    pdata.scale = 3.f;
    pdata.isSensor = true;
    pdata.type = ProjectileTypeDefenderShield;

    Projectile *pj;
    for (int i = 0; i < PROJECTILES; i++)
    {
        pj = SpawnProjectile(
            pdata, getPositionBT() +
            btVector3(rng.getRandomFloat(-5.f, 5.f), rng.getRandomFloat(-4.0f, 5.f), rng.getRandomFloat(-5.f, 5.f)),
            btVector3(0.f, 0.f, 0.f),
            *this
        );

        if (pj)
        {
            m_projectiles.push_back(pj);
            increaseCallbackEntities();
        }
    }
}

void EnemyDefender::createAbilities()
{
    AbilityData data;
    data.duration = 1250.f;
    data.randomChanche = 0;
    data.cooldown = 1500.f;
    m_melee = Ability(data, [&](Player &player, Ability &ab) -> void { // ontick
        if ((player.getPositionBT() - getPositionBT()).length() <= MELEE_DISTANCE &&
            ab.getCurrentDuration() <= 0.f)
        {
            player.takeDamage(getBaseDamage());
        }
    }, [&](Player &player, Ability &ab) -> void { // on use

    });
}

void EnemyDefender::clear()
{
}

void EnemyDefender::onCollision(PhysicsObject &other, btVector3 contactPoint, float dmgMultiplier)
{
    if (Projectile *pj = dynamic_cast<Projectile*> (&other))
    {
        if (!pj->getProjectileData().enemyBullet)
        {
            if (m_projectiles.empty())
            {
                damage(static_cast<int> (pj->getProjectileData().damage * dmgMultiplier));

                if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                    getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME));
            }
            else
            {
                Projectile *def = m_projectiles[m_projectiles.size() - 1];
                def->start(
                    (def->getPositionBT() - getPositionBT() + getRigidBody()->getLinearVelocity()).normalized(),
                    getStatusManager()
                );
                def->getProjectileData().ttl = 0;
                m_projectiles.pop_back();
                decreaseCallbackEntities();

                pj->getRigidBody()->setLinearVelocity(- pj->getRigidBody()->getLinearVelocity());
            }
        }
    }
}

void EnemyDefender::updateSpecific(Player &player, float deltaTime)
{
    RandomGenerator &rng = RandomGenerator::singleton();
    m_defenseTime += deltaTime * 0.005f;

    for (int i = 0; i < m_projectiles.size(); i++)
    {
        m_projectiles[i]->getRigidBody()->getWorldTransform().setOrigin(getPositionBT() +
        btVector3(
            std::cos(m_defenseTime + i) * getHalfExtent().x() * 2,
            (i - std::floor(PROJECTILES / 2)) * 0.8f,
            std::sin(m_defenseTime + i) * getHalfExtent().z() * 2
        ));
    }
}

void EnemyDefender::updateDead(float deltaTime)
{
}

void EnemyDefender::useAbility(Player &target)
{
    if ((target.getPositionBT() - getPositionBT()).length() <= MELEE_DISTANCE)
        m_melee.useAbility(target);
}