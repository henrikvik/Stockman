#include <AI\EnemyTotem.h>
#include <Projectile\Projectile.h>
#include <Misc\ComboMachine.h>
using namespace Logic;

const float EnemyTotem::BASE_SPEED = 0.f, EnemyTotem::BULLET_SPEED = 55.f;
const float EnemyTotem::AB_SCALE = 8.5f, EnemyTotem::AB_ROTATION = 0.2f;

const int EnemyTotem::BASE_DAMAGE = 1, EnemyTotem::MAX_HP = 550, EnemyTotem::SCORE = 50;
const int EnemyTotem::BULLET_AMOUNT = 9;

EnemyTotem::EnemyTotem(btRigidBody * body, btVector3 halfExtent)
    : Enemy(Resources::Models::Files::StaticSummon, body, halfExtent, MAX_HP, BASE_DAMAGE, BASE_SPEED, EnemyType::TOTEM, 0, btVector3(0, 0, 0))
{
    setBehavior(STAY);
    createAbilities();
    m_rotation = 0;

    addCallback(ON_DEATH, [&](CallbackData data) -> void {
        ComboMachine::Get().kill(SCORE);
        SpawnTrigger(2, getPositionBT() - btVector3(
            0.f, data.caller->getRigidBody()->getCollisionShape()->getLocalScaling().y() * 1.5f, 0.f),
            std::vector<int>{ StatusManager::AMMO_PICK_UP_PRIMARY 
        });
    });
}

EnemyTotem::~EnemyTotem()
{
}

void EnemyTotem::createAbilities()
{
    static Graphics::ParticleEffect necroTrail = Graphics::FXSystem->getEffect("NecroProjTrail");

    AbilityData data;
    data.duration = 0.f;
    data.cooldown = 500.f;
    data.randomChanche = 2;

    spreadShot = Ability(data, [&](Player &target, Ability &ab) -> void {

    }, [&](Player &target, Ability &ab) -> void {
        constexpr float piece = 3.14 * 2 / BULLET_AMOUNT;

        ProjectileData pData;
        
        pData.effect = necroTrail;
        pData.hasEffect = true;
        pData.effectVelocity = false;
        pData.effectActivated = true;
        m_rotation += AB_ROTATION;

        for (int i = 0; i < BULLET_AMOUNT; i++)
        {
            shoot(btVector3(std::sin((i + m_rotation) * piece), 0.f,
                std::cos((i + m_rotation) * piece)), pData, BULLET_SPEED, 0.f, AB_SCALE);
        }
    });
}

void EnemyTotem::clear()
{
}

void EnemyTotem::onCollision(PhysicsObject &other, btVector3 contactPoint, float dmgMultiplier)
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
                getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME));
        }
    }
}

void EnemyTotem::updateSpecific(Player & player, float deltaTime)
{
    spreadShot.update(deltaTime, player);
}

void EnemyTotem::updateDead(float deltaTime)
{
}

void EnemyTotem::useAbility(Player &target)
{
    spreadShot.useAbility(target);
}