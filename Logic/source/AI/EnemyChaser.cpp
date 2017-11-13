#include <AI\EnemyChaser.h>
#include <Player\Player.h>
#include <Projectile\Projectile.h>
using namespace Logic;

const int EnemyChaser::MAX_HP = 25;
const int EnemyChaser::BASE_DAMAGE = 1;
const float EnemyChaser::MOVE_SPEED = 10;

EnemyChaser::EnemyChaser(btRigidBody* body)
    : Enemy(Resources::Models::Files::StaticSummon, body, { 1, 1, 1 },
        MAX_HP, BASE_DAMAGE, MOVE_SPEED, EnemyType::NECROMANCER_MINION, 0) // use in para instead note
{
    setBehavior(MELEE);
    getSoundSource()->playSFX(Sound::SFX::NECROMANCER_SPAWN);
    getSoundSource()->autoPlaySFX(Sound::SFX::FOOTSTEP_SMALL, 150, 75, 1.f, 0.10f);
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