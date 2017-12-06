#include <AI\EnemySoarer.h>
#include <Projectile\Projectile.h>
#include <Misc\ComboMachine.h>
#include <AI\Behavior\Behavior.h>
using namespace Logic;

const int EnemySoarer::HEALTH = 375, EnemySoarer::DAMAGE = 1, EnemySoarer::SCORE = 150;
const float EnemySoarer::SPEED = 20.f,
            EnemySoarer::STEERING_MOD = 1.4f,
            EnemySoarer::AB1_SPEED = 25.f,
            EnemySoarer::AB1_SCALE = 13.f,
            EnemySoarer::AB1_GRAVITY = 6.5f,
            EnemySoarer::HEIGHT_OFFSET = 15.f;

EnemySoarer::EnemySoarer(btRigidBody *body, btVector3 halfExtent)
    : Enemy(Resources::Models::StaticSummon, body, halfExtent,
            HEALTH, DAMAGE, SPEED, EnemyType::FLYING, 0) 
{
    addCallback(ON_DEATH, [&](CallbackData data) -> void {
        ComboMachine::Get().kill(SCORE);
    });

    setBehavior(MELEE);
    createAbilities();
    body->translate({ 0, HEIGHT_OFFSET, 0 });

    gravity = getRigidBody()->getGravity();

    getSoundSource()->autoPlaySFX(Sound::SFX::ENEMY_AMBIENT_1, 6500, 500, 1.f, 0.10f);
    light.color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f);
    light.intensity = 0.8f;
    light.range = 5.0f;

    getBehavior()->setSteeringSpeed(getBehavior()->getSteeringSpeed() * STEERING_MOD);
}

EnemySoarer::~EnemySoarer()
{
}

void EnemySoarer::createAbilities()
{
      // ab 1
    AbilityData data;

    data.cooldown = 1100.f;
    data.duration = 0.f;
    data.randomChanche = 3;

    static Graphics::ParticleEffect necroTrail = Graphics::FXSystem->getEffect("DamageProjTrail");
    ProjectileData pdata;
    pdata.effect = necroTrail;
    pdata.hasEffect = true;
    pdata.effectVelocity = false;
    pdata.effectActivated = true;


    ab1 = Ability(data, [&](Player &player, Ability &ab) -> void {
        // ontick 
    }, [=](Player &player, Ability &ab) -> void {
        // onuse
        auto pj = shoot({ 0, -1, 0 }, pdata, AB1_SPEED, AB1_GRAVITY, AB1_SCALE);
        getSoundSource()->playSFX(Sound::SFX::WEAPON_ICEGUN_PRIMARY, 1.f, 0.15f);
        if (pj) {
            pj->addCallback(ON_COLLISION, [&](CallbackData &data) -> void {
                Graphics::FXSystem->addEffect("DamageBoom", data.caller->getPosition());

                data.caller->getSoundSource()->playSFX(Sound::SFX::WEAPON_ICEGUN_SECONDARY, 1.f, 0.15);
            });
        }
    });
}

void EnemySoarer::onCollision(PhysicsObject &other, btVector3 contactPoint, float dmgMultiplier)
{
    if (Projectile *pj = dynamic_cast<Projectile*> (&other))
    {
        if (!pj->getProjectileData().enemyBullet)
        {
            damage(static_cast<int> (pj->getProjectileData().damage * dmgMultiplier));

            if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::BULLET_TIME,
                    pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME));
        }
    }
}

void EnemySoarer::updateSpecific(Player &player, float deltaTime)
{
    ab1.useAbility(player); // move to use ability and make seperate behavior tree
    ab1.update(deltaTime, player);

    handleFlying(player);
}

void EnemySoarer::updateDead(float deltaTime)
{
}

void EnemySoarer::useAbility(Player &target)
{
}

void EnemySoarer::handleFlying(Player const &target)
{
    getRigidBody()->setGravity(btVector3(0.f, 0.f, 0.f));
    auto vel = getRigidBody()->getLinearVelocity();

    if (getPositionBT().y() > HEIGHT_OFFSET + target.getPositionBT().y())
        vel.setY(-0.25f);
    else if (getPositionBT().y() < (HEIGHT_OFFSET + target.getPositionBT().y()) * 0.9f)
        vel.setY(0.25f);

    getRigidBody()->setLinearVelocity(vel);
}
