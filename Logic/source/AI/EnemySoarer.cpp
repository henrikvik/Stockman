#include <AI\EnemySoarer.h>
#include <Projectile\Projectile.h>
using namespace Logic;

const int EnemySoarer::HEALTH = 400, EnemySoarer::DAMAGE = 1;
const float EnemySoarer::SPEED = 15.f, 
            EnemySoarer::AB1_SPEED = 20.f, EnemySoarer::AB2_SPEED = 15.f, AB2_SPEED_Y = 2.5f;

EnemySoarer::EnemySoarer(btRigidBody *body, btVector3 halfExtent)
    : Enemy(Resources::Models::StaticSummon, body, halfExtent,
            HEALTH, DAMAGE, SPEED, EnemyType::UNNAMED_1, 0) 
{
    setBehavior(MELEE);
    createAbilities();

    gravity = getRigidBody()->getGravity();

    addCallback(ON_DAMAGE_TAKEN, [&](CallbackData &data) -> void {
        getSoundSource()->playSFX(Sound::SFX::JUMP, 8.5f, 1.f);
    });
}

EnemySoarer::~EnemySoarer()
{
}

void EnemySoarer::createAbilities()
{
    AbilityData data;
    data.cooldown = 12000.f;
    data.duration = 15000.f;
    data.randomChanche = 0;

    // ab 2
    ab2 = Ability(data, [&](Player &player, Ability &ab) -> void {
        // ontick0
        player.getCharController()->warp(getPositionBT() - btVector3{0, 1, 0} * btScalar(5.f));
        getRigidBody()->setLinearVelocity(dir * btScalar(ab2Speed) + btVector3{ 0, 1, 0 } * btScalar(AB2_SPEED));

        if (ab.getCurrentDuration() <= 0.f)
        {
            player.resetTargeted();
        } 
    }, [&](Player &player, Ability &ab) -> void {
        // onuse
        dir = btVector3(RandomGenerator::singleton().getRandomFloat(0.1f, 0.2f),
            0.f, RandomGenerator::singleton().getRandomFloat(0.1f, 0.2f));
        dir.normalize();

        ab2Speed = AB2_SPEED;
        player.setTargetedBy(this);
    });

    // ab 1
    data.cooldown = 2000.f;
    data.duration = 0.f;
    data.randomChanche = 4;

    ab1 = Ability(data, [&](Player &player, Ability &ab) -> void {
        // ontick 
    }, [&](Player &player, Ability &ab) -> void {
        // onuse
        shoot({ 0, -1, 0 }, Resources::Models::Files::CrossBowProjectile, AB1_SPEED, 5.f, 2.5f);
    });
}

void EnemySoarer::onCollision(PhysicsObject &other, btVector3 contactPoint, float dmgMultiplier)
{
    if (Player *player = dynamic_cast<Player*>(&other))
    {
        if (!player->isTargeted())
            ab2.useAbility(*player);
    }
    else if (Projectile *pj = dynamic_cast<Projectile*> (&other))
    {
        if (!pj->getProjectileData().enemyBullet)
        {
            damage(static_cast<int> (pj->getProjectileData().damage * dmgMultiplier));

            if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME,
                    pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
        }
    }
}

void EnemySoarer::updateSpecific(Player &player, float deltaTime)
{
    if (!ab2.isUsingAbility())
    {
        ab1.update(deltaTime, player);
        btVector3 to = player.getPositionBT() - getPositionBT();
        to.setY(0);

        if (to.length() > 25.f && !player.isTargeted())
            getRigidBody()->setGravity(btVector3(0.f, 0.f, 0.f));
        else
            getRigidBody()->setGravity(gravity);
    }
    else 
    {
        ab2Speed += deltaTime * 0.001f;
        ab2.update(deltaTime, player);
    }
}

void EnemySoarer::updateDead(float deltaTime)
{
}

void EnemySoarer::useAbility(Player &target)
{
}