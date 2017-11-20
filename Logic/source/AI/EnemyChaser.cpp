#include <AI\EnemyChaser.h>
#include <Player\Player.h>
#include <Projectile\Projectile.h>
using namespace Logic;

const int EnemyChaser::MAX_HP = 25;
const int EnemyChaser::BASE_DAMAGE = 1;
const float EnemyChaser::MOVE_SPEED = 12.f;
#define ANI_TIME 5000.f // this is temp

EnemyChaser::EnemyChaser(btRigidBody* body)
    : Enemy(Resources::Models::Files::SummonUnitWithAnim, body, { 1.0f, 1.0f, 1.0f },
        MAX_HP, BASE_DAMAGE, MOVE_SPEED, EnemyType::NECROMANCER_MINION, 0, { 0.f, -0.8f, 0.f }) // use in para instead note
{
    setBehavior(MELEE);
    getSoundSource()->playSFX(Sound::SFX::NECROMANCER_SPAWN);
    getSoundSource()->autoPlaySFX(Sound::SFX::FOOTSTEP_SMALL, 150, 75, 1.f, 0.10f);
    loadAnimation(Resources::Models::Files::SummonUnitWithAnim);
    light.color = DirectX::SimpleMath::Color(0.0f, 0.7f, 1.0f);
    light.intensity = 0.6f;
}

EnemyChaser::~EnemyChaser()
{
}

void EnemyChaser::loadAnimation(Resources::Models::Files model)
{
    animatedRenderInfo.animationName = "Walk";
    animatedRenderInfo.model = model;
    animatedRenderInfo.animationTimeStamp = 0.0f;
    animatedRenderInfo.transform = getTransformMatrix();
}

// REMOVE WHEN ALL ENIMES ARE ANIMATED
void EnemyChaser::updateSpecific(Player & player, float deltaTime)
{
    animatedRenderInfo.transform = getModelTransformMatrix();
    animatedRenderInfo.animationTimeStamp += deltaTime;
    if (animatedRenderInfo.animationTimeStamp > ANI_TIME) animatedRenderInfo.animationTimeStamp = 0.f;
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
                    getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME));
            }
        }
        else if (Player *p = dynamic_cast<Player*> (&other))
        {
            p->takeDamage(getBaseDamage());
            damage(getHealth());
        }
    }
}

void EnemyChaser::renderSpecific() const
{
    QueueRender(animatedRenderInfo);
}