#include <AI\EnemySoarer.h>
#include <Projectile\Projectile.h>
using namespace Logic;

const int EnemySoarer::HEALTH = 400, EnemySoarer::DAMAGE = 1;
const float EnemySoarer::SPEED = 15.f, 
            EnemySoarer::AB1_SPEED = 20.f,
            EnemySoarer::HEIGHT_OFFSET = 20.f;

EnemySoarer::EnemySoarer(btRigidBody *body, btVector3 halfExtent)
    : Enemy(Resources::Models::StaticSummon, body, halfExtent,
            HEALTH, DAMAGE, SPEED, EnemyType::FLYING, 0) 
{
    setBehavior(MELEE);
    createAbilities();
    body->translate({ 0, HEIGHT_OFFSET, 0 });

    gravity = getRigidBody()->getGravity();

    addCallback(ON_DAMAGE_TAKEN, [&](CallbackData &data) -> void {
        getSoundSource()->playSFX(Sound::SFX::JUMP, 8.5f, 1.f);
    });

    light.color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f);
    light.intensity = 0.8f;
    light.range = 5.0f;
}

EnemySoarer::~EnemySoarer()
{
}

void EnemySoarer::createAbilities()
{
      // ab 1
    AbilityData data;

    data.cooldown = 1300.f;
    data.duration = 0.f;
    data.randomChanche = 4;

    ab1 = Ability(data, [&](Player &player, Ability &ab) -> void {
        // ontick 
    }, [&](Player &player, Ability &ab) -> void {
        // onuse
        shoot({ 0, -1, 0 }, Resources::Models::Files::Crossbowbolt, AB1_SPEED, 5.f, 3.5f);
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

    getRigidBody()->setGravity(btVector3(0.f, 0.f, 0.f));
    if (getPositionBT().y() > HEIGHT_OFFSET) // bad fix but better to just force it right now
        getRigidBody()->getWorldTransform().getOrigin().setY(HEIGHT_OFFSET);
    else if (getPositionBT().y() < HEIGHT_OFFSET * 0.8f)
        getRigidBody()->setLinearVelocity(getRigidBody()->getLinearVelocity() + btVector3(0.f, 1.f, 0.f));
}

void EnemySoarer::updateDead(float deltaTime)
{
}

void EnemySoarer::useAbility(Player &target)
{
}