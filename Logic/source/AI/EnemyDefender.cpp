#include <AI\EnemyDefender.h>
#include <Misc\ComboMachine.h>
#include <Projectile\Projectile.h>
using namespace Logic;

const float EnemyDefender::BASE_SPEED = 14.f;
const float EnemyDefender::MELEE_DISTANCE = 16.5f,
            EnemyDefender::PROJECTILE_SPEED = 115.f,
            EnemyDefender::THROW_STRENGTH = 0.05f;

const int   EnemyDefender::BASE_DAMAGE = 1,
            EnemyDefender::MAX_HP = 100,
            EnemyDefender::MAX_DEF_HP = 4,
            EnemyDefender::SCORE = 100,
            EnemyDefender::PROJECTILES = 20,
            EnemyDefender::INDICATORS = 12;

EnemyDefender::EnemyDefender(btRigidBody *body, btVector3 halfExtent)
    : Enemy(Resources::Models::Grunt, body, halfExtent, MAX_HP,
        BASE_DAMAGE, BASE_SPEED, EnemyType::DEFENDER, 0, { 0.4f, -2.4f, -0.2f }) {
    setBehavior(MELEE);
    createAbilities();

    m_defenseTime = 0.f;
    m_defenseHealth = MAX_DEF_HP;

    getAnimatedModel().set_next("Run_Grunt");

    light.color = DirectX::SimpleMath::Color(0.7f, 0.0f, 1.0f);
    light.intensity = 0.6f;
    light.range = 15.0f;

    m_meleeIndicators.resize(INDICATORS);
    addCallback(ON_DEATH, [&](CallbackData &data) -> void {
        for (Projectile *pj : m_meleeIndicators) {
            if (pj) {
                pj->setDead(true);
            }
        }
    });
}

EnemyDefender::~EnemyDefender()
{

}

void EnemyDefender::onSpawn()
{
    m_projectiles.reserve(PROJECTILES);
    addCallback(ON_DEATH, [&](CallbackData data) -> void {
        ComboMachine::Get().kill(SCORE);
        if (RandomGenerator::singleton().getRandomInt(0, 2))
        {
            RandomGenerator::singleton().getRandomInt(0, 1) ?
                SpawnTrigger(2, getPositionBT() - btVector3(0.f, data.caller->getRigidBody()->getCollisionShape()->getLocalScaling().y() * 1.5f, 0.f), std::vector<int>{ StatusManager::AMMO_PICK_UP_PRIMARY }) :
                SpawnTrigger(3, getPositionBT() - btVector3(0.f, data.caller->getRigidBody()->getCollisionShape()->getLocalScaling().y() * 1.5f, 0.f), std::vector<int>{ StatusManager::AMMO_PICK_UP_SECONDARY });
        }
        
    });

    ProjectileData pdata;
    RandomGenerator &rng = RandomGenerator::singleton();

    pdata.damage = 0;
    pdata.enemyBullet = true;
    pdata.ttl = 999999; // is destroyed manually
    pdata.meshID = Resources::Models::Bone;
    pdata.speed = PROJECTILE_SPEED;
    pdata.scale = 1.f;
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
    data.duration = 3000.f;
    data.randomChanche = 0;
    data.cooldown = 3500.f;
    m_melee = Ability(data, [&](Player &player, Ability &ab) -> void { // ontick
        constexpr float piece = 3.14f / INDICATORS * 2;

        for (int i = 0; i < m_meleeIndicators.size(); i++) {
            if (m_meleeIndicators[i]) {
                float scl = 1.f - (ab.getCurrentDuration() / ab.getData().duration);
                m_meleeIndicators[i]->getRigidBody()->getWorldTransform().setOrigin(
                    getPositionBT() + btVector3(std::cos(i * piece) * MELEE_DISTANCE * scl,
                                                0.5f,
                                                std::sin(i * piece) * MELEE_DISTANCE * scl
                                               ));
            }
        }

        if (ab.getCurrentDuration() <= 0.f && 
            (player.getPositionBT() - getPositionBT()).length() <= MELEE_DISTANCE)
        {
            player.takeDamage(getBaseDamage());

            btVector3 diff = player.getPositionBT() - getPositionBT();
            diff.setY(0);
            player.getCharController()->applyImpulse(diff.normalized() * THROW_STRENGTH);
            player.getStatusManager().addStatus(StatusManager::SHIELD_CHARGE, 1); // test

            for (Projectile *pj : m_meleeIndicators) {
                if (pj) {
                    pj->setDead(true);
                }
            }
        }
    }, [&](Player &player, Ability &ab) -> void { // on use
        getAnimatedModel().set_next("Attack_Grunt", [&]() -> void {
            getAnimatedModel().set_delta_multiplier(getAnimatedModel().get_animation_time() * 1000.f / (ab.getCurrentDuration()) - 0.169f); // noise
            getAnimatedModel().set_next("Run_Grunt", [&]() -> void {
                getAnimatedModel().set_delta_multiplier(1.f);
            });
        });

        // TEST INDICATORS [REPLACE]
        ProjectileData pdata;
        pdata.damage = 0;
        pdata.enemyBullet = true;
        pdata.ttl = ab.getData().duration * 0.95f;
        pdata.meshID = Resources::Models::Bone;
        pdata.speed = PROJECTILE_SPEED;
        pdata.scale = 1.f;
        pdata.isSensor = true;
        pdata.shouldRender = true;
        pdata.type = ProjectileTypeDefenderShield;

        for (int i = 0; i < INDICATORS; i++) {
            Projectile *pj = SpawnProjectile(pdata, btVector3(0, 0, 0), btVector3(0, 0, 0), *this);
            m_meleeIndicators[i] = pj;
            if (pj) {
                increaseCallbackEntities();
                pj->addCallback(ON_DESTROY, [=](CallbackData &data) {
                    m_meleeIndicators[i] = nullptr;
                    decreaseCallbackEntities();
                });
            }
        }
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
            if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
            {
                getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME));
            }
            else
            {
                if (m_projectiles.empty())
                {
                    damage(static_cast<int> (pj->getProjectileData().damage * dmgMultiplier));
                }
                else
                {
                    onDefenseCollision(pj);
                }
            }
        }
    }
}

// called when a projectile collision happens if the defender still have defense
void EnemyDefender::onDefenseCollision(Projectile * pj)
{
    m_defenseHealth--;

    if (m_defenseHealth <= 0)
    {
        Projectile *def = m_projectiles[m_projectiles.size() - 1];
        def->start(
            (def->getPositionBT() - getPositionBT() + getRigidBody()->getLinearVelocity()).normalized(),
            getStatusManager()
        );
        def->getProjectileData().ttl = 0;
        m_projectiles.pop_back();
        decreaseCallbackEntities();

        m_defenseHealth = MAX_DEF_HP;
    }

    pj->setDead(true);
}

void EnemyDefender::damage(int damage)
{
    if (m_projectiles.empty()) {
        Enemy::damage(damage);
        if (damage > 0)
            SpawnDamageText(damage, DirectX::Colors::FloralWhite);
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

    m_melee.update(deltaTime, player);
}

void EnemyDefender::updateDead(float deltaTime)
{
}

void EnemyDefender::useAbility(Player &target)
{
    if ((target.getPositionBT() - getPositionBT()).length() <= MELEE_DISTANCE)
        m_melee.useAbility(target);
}