#include <AI\EnemyChaser.h>
#include <Player\Player.h>
#include <Projectile\Projectile.h>
#include <Misc\ComboMachine.h>
using namespace Logic;

const int EnemyChaser::MAX_HP = 25;
const int EnemyChaser::BASE_DAMAGE = 1;
const float EnemyChaser::MOVE_SPEED = 16.9f; //noice!

EnemyChaser::EnemyChaser(btRigidBody* body)
    : Enemy(Resources::Models::Files::SummonUnitWithAnim, body, { 1.0f, 1.0f, 1.0f },
        MAX_HP, BASE_DAMAGE, MOVE_SPEED, EnemyType::NECROMANCER_MINION, 0, { 0.f, 0.1f, 0.f }) // use in para instead note
{
    setBehavior(MELEE);
    getSoundSource()->playSFX(Sound::SFX::NECROMANCER_SPAWN);
    getSoundSource()->autoPlaySFX(Sound::SFX::FOOTSTEP_SMALL, 333, 10, 1.f, 0.15f);
    light.color = DirectX::SimpleMath::Color(0.0f, 0.7f, 1.0f);
    light.intensity = 1.f;
    light.range = 3.f;

    getAnimatedModel().set_next("Walk", [&]()->void {
        getAnimatedModel().set_delta_multiplier(1 / 299.f); 
    });

    light.color = DirectX::SimpleMath::Color(0.1f, 0.7f, 0.4f);
    light.intensity = 0.4f;
    light.range = 5.f;
}

EnemyChaser::~EnemyChaser()
{

}

void EnemyChaser::updateSpecific(Player & player, float deltaTime)
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

void EnemyChaser::updateAnimation(float deltaTime)
{
    animatedModel.set_transform(getModelTransformMatrix());
    animatedModel.update(deltaTime);
}