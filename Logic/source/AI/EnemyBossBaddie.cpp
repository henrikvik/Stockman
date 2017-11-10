#include <AI\EnemyBossBaddie.h>
#include <Player\Player.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Graphics\include\Structs.h>
using namespace Logic;

const float EnemyBossBaddie::BASE_SPEED = 1.5f;
const int EnemyBossBaddie::BASE_DAMAGE = 1, EnemyBossBaddie::MAX_HP = 10000; // though guy, for you

EnemyBossBaddie::EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent)
    : Enemy(Resources::Models::UnitCube, body,
    halfExtent, MAX_HP, BASE_DAMAGE, BASE_SPEED, EnemyType::BOSS_1, 0)
{
    setBehavior(BOSS_BADDIE);
    getSoundSource()->playMusic(Sound::MUSIC::BOSS_1_MUSIC_1);
    createAbilities();
}

/*

    THIS METHOD CREATES THE ABILITES; CHANGE THEM HERE!

*/
void EnemyBossBaddie::createAbilities()
{
    AbilityData data;

    data.cooldown = 12500.f; // Ability One Data
    data.duration = 9000.f;
    data.randomChanche = 0;

    auto onUse = [&](Player& player, Ability &ability) -> void {
        player.getSoundSource()->playSFX(Sound::SFX::BOSS_1_ABILITY_1);
    };

    auto onTick = [&](Player& player, Ability &ability) -> void {
        btVector3 force = (player.getPositionBT() - getPositionBT()) *
            (1 + ability.getCurrentDuration() - ability.getData().duration) * 0.00000001f;
        player.getCharController()->applyImpulse(force);
    };

    abilities[AbilityId::ONE] = Ability(data, onTick, onUse);
}

void EnemyBossBaddie::useAbility(Player &target, int phase)
{
    switch (phase)
    {
        case 0:
            abilities[AbilityId::ONE].useAbility(target);
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
    // test
    useAbility(player, 0);
    for (auto pair : abilities)
        pair.second.update(deltaTime, player);
}

void EnemyBossBaddie::updateDead(float deltaTime)
{
}