#include <AI\EnemyChaser.h>
#include <Projectile\Projectile.h>
using namespace Logic;

const float EnemyChaser::MAX_HP = 3;
const float EnemyChaser::BASE_DAMAGE = 5;
const float EnemyChaser::MOVE_SPEED = 18;

EnemyChaser::EnemyChaser(btRigidBody* body)
    : Enemy(Resources::Models::Files::UnitCube, body, { 1, 1, 1 }, MAX_HP, BASE_DAMAGE, MOVE_SPEED, NECROMANCER_MINION, 0) // use in para instead note
{
    setBehavior(MELEE);
}

EnemyChaser::~EnemyChaser()
{
}

void EnemyChaser::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    if (Projectile *pj = dynamic_cast<Projectile*> (&other))
    {
        if (!pj->getProjectileData().enemyBullet)
        {
            damage(pj->getProjectileData().damage * dmgMultiplier);

            if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
        }
    }
}