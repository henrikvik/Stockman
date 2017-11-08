#include <AI\EnemyBossBaddie.h>
#include <Player\Player.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Graphics\include\Structs.h>
using namespace Logic;

const float EnemyBossBaddie::BASE_SPEED = 1.5f, EnemyBossBaddie::ABILITY_1_DURATION = 10000.f;
const int EnemyBossBaddie::BASE_DAMAGE = 1, EnemyBossBaddie::MAX_HP = 1000; // though guy, for you

EnemyBossBaddie::EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent)
    : Enemy(Graphics::ModelID::ENEMYGRUNT, body,
    halfExtent, MAX_HP, BASE_DAMAGE, BASE_SPEED, EnemyType::BOSS_1, 0)
{
    setBehavior(BOSS_BADDIE);

    abilityTimer[0] = ABILITY_1_DURATION;
    usingAbility[0] = false;
}

void EnemyBossBaddie::useAbility(Entity const &target, int phase)
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

void EnemyBossBaddie::updateSpecific(Player const &player, float deltaTime)
{
    //TESTING TESTING TESTING
    Player &player2 = const_cast<Player&> (player); // This is evil anakin! Not from my perspective.
    if (usingAbility[0])
    {
        player2.getCharController()->applyImpulse((getPositionBT() - player.getPositionBT()) * 100.f * (abilityTimer[0] - ABILITY_1_DURATION + 1));

        abilityTimer[0] -= deltaTime;
        if (abilityTimer[0] < 0)
            usingAbility[0] = false;
    }
    if (!usingAbility[0]) useAbility1(player);
}

void EnemyBossBaddie::updateDead(float deltaTime)
{
}

void EnemyBossBaddie::useAbility1(Entity const &target)
{
    //Sound::NoiseMachine::Get().playSFX(Sound::SFX::BOSS_1_ABILITY_1, const_cast<Entity&> (target).getSoundSource(), true);
    abilityTimer[0] = ABILITY_1_DURATION;
    usingAbility[0] = true;
}