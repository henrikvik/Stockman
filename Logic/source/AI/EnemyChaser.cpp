#include <AI\EnemyChaser.h>
#include <Player\Player.h>
#include <Projectile\Projectile.h>
using namespace Logic;

const int EnemyChaser::MAX_HP = 3;
const int EnemyChaser::BASE_DAMAGE = 1;
const float EnemyChaser::MOVE_SPEED = 13;

EnemyChaser::EnemyChaser(btRigidBody* body)
    : Enemy(Graphics::ModelID::CUBE, body, { 1, 1, 1 }, MAX_HP, BASE_DAMAGE, MOVE_SPEED, EnemyType::NECROMANCER_MINION, 0) // use in para instead note
{
    setBehavior(MELEE);
}

EnemyChaser::~EnemyChaser()
{
}

void EnemyChaser::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    if (getHealth() > 0)
    {
        if (Projectile *pj = dynamic_cast<Projectile*> (&other))
        {
            if (!pj->getProjectileData().enemyBullet)
            {
                damage(static_cast<int> (pj->getProjectileData().damage * dmgMultiplier));

                if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                    getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
            }
        }
        else if (Player *p = dynamic_cast<Player*> (&other))
        {
            p->takeDamage(getBaseDamage());
            damage(getHealth());
        }
    }
}