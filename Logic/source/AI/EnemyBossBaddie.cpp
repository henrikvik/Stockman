#include <AI\EnemyBossBaddie.h>
#include <Player\Player.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Graphics\include\Structs.h>
using namespace Logic;

const float EnemyBossBaddie::BASE_SPEED = 1.5f, EnemyBossBaddie::ABILITY_1_DURATION = 10000.f;
const int EnemyBossBaddie::BASE_DAMAGE = 1, EnemyBossBaddie::MAX_HP = 10000; // though guy, for you

EnemyBossBaddie::EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent)
    : Enemy(Resources::Models::UnitCube, body,
    halfExtent, MAX_HP, BASE_DAMAGE, BASE_SPEED, EnemyType::BOSS_1, 0)
{
    setBehavior(BOSS_BADDIE);
    getSoundSource()->playMusic(Sound::MUSIC::BOSS_1_MUSIC_1);

    abilityTimer[0] = ABILITY_1_DURATION;
    usingAbility[0] = false;
}

void EnemyBossBaddie::useAbility(Entity &target, int phase)
{
    switch (phase)
    {
        case 0:
            useAbility1(target);
            break;
        case 1:
            break;
        case 2:
            break;
    }
}

void EnemyBossBaddie::onCollision(PhysicsObject &other, btVector3 contactPoint, float dmgMultiplier)
{
}

void EnemyBossBaddie::updateSpecific(Player &player, float deltaTime)
{
    if (usingAbility[0])
    {
        btVector3 force = (player.getPositionBT() - getPositionBT()) * (1 + abilityTimer[0] - ABILITY_1_DURATION) * 0.00000001f;
        player.getCharController()->applyImpulse(force);

        abilityTimer[0] -= deltaTime;
        if (abilityTimer[0] < 0)
            usingAbility[0] = false;
    }
    
    if (!usingAbility[0]) useAbility1(player);
}

void EnemyBossBaddie::updateDead(float deltaTime)
{
}

void EnemyBossBaddie::useAbility1(Entity &target)
{
    target.getSoundSource()->playSFX(Sound::SFX::BOSS_1_ABILITY_1);
    abilityTimer[0] = ABILITY_1_DURATION;
    usingAbility[0] = true;
}