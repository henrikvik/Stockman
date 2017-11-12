#include <AI\EnemyBossBaddie.h>

#include <Misc\Sound\NoiseMachine.h>
#include <Player\Player.h>
#include <Projectile\Projectile.h>
#include <sstream>

#include <Graphics\include\Structs.h>
using namespace Logic;

const float EnemyBossBaddie::BASE_SPEED = 4.5f, EnemyBossBaddie::PROJECTILE_SPEED = 35.f,
            EnemyBossBaddie::ABILITY_1_MOD = 0.085f, EnemyBossBaddie::MELEE_RANGE = 27.5f,
            EnemyBossBaddie::MELEE_PUSHBACK = 0.105f;
const int EnemyBossBaddie::BASE_DAMAGE = 1, EnemyBossBaddie::MAX_HP = 10000; // Big guy, for you
#define NECRO_COUNT 3

EnemyBossBaddie::EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent)
    : Enemy(Resources::Models::UnitCube, body,
        halfExtent, MAX_HP, BASE_DAMAGE, BASE_SPEED, EnemyType::BOSS_1, 0)
{
    setBehavior(BOSS_BADDIE);

    ability3Offset = 0;

    Sound::NoiseMachine::Get().stopAllGroups();
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::BOSS_1_MUSIC_1, nullptr, true);
    createAbilities();
    
    addCallback(ON_DEATH, [&](CallbackData &data)
    {
        Sound::NoiseMachine::Get().stopAllGroups();
        Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::BOSS_1_MUSIC_2, nullptr, true);
    });

    // test
    std::wstring test = L"Created By: Stockman Games Entertainment\n\nProgrammers:\nAndreas Henriksson\nHenrik Vik\nJakob Nyberg\nSimon Fredholm\nLukas Westling\nEmanuel Bjurman\nFelix Kaaman\nJohan & Simon\n\nTechnical Artists: Johan & Simon\n\nThe Bad Format: .lw\n\nGitmeister: Henrik Vik\n\nInvestor: Gabe Newell\n\nThe guy that fixed everyone elses stuff: Henrik Vik\n\nGame Manager & Producer: Henrik Vik\n\nWonderful Music: Banana\n\n\nThank you\nfor playing!\n\n\n\n\n\n\n\n\n\n\n\n\nEnemies:\nNecromancer (The annoying dude)\nNecromancer Minion (The dude everyone wants to nerf)\nThe Boss (The boss everyone hates)\nTorpedo Ted\nReznor, The Secret boss.\nLarry Koopa\nLemmy Koopa\nMorton Koopa Jr.\nThe Lich King\nDoctor Boom\nGrim Patron\nThe Hogger\nHanzo Mains\nVampires from Castle Wars\nOP Ferie Dragons\nKalphite Queen\nDr Stockman\nMr King Dice\nRace Conditions\nDeadlines\nPlaytesting\nStandup meetings\nMagic numbers\nHealthbars\nBowser Baloon Minigame from mario party 4 and 3\nWow: Classic Servers\nChimaeron\nMitch McConnell (turtle guy)\nIce Poseidon\nMr Garrison (President Of The United States Of America)\nProfessor Chaos\nKyle Brofloski\nStan Marsh\nRandy Marsh\nHeidi Turner :(\nPasha Biceps\nOlofmeister\nFriberg\nMoonmoon\nTwitch ads every fucking second on the yugioh stream\nH3h3productions\nKennyS\nNaniwa\n\n\nGot this far without cheats or crashes? nice.\nSpecial Thanks to: Henrik Vik!\n\n\n\nStockman studio is not responsible for any crashes or bugs that might damage your computer."; // See it in game not here dude :>
    std::wstringstream str(test);
    std::wstring temp;
    float x = 300.f, y = 715.5f;
    infoText.reserve(150);
    while (getline(str, temp, L'\n'))
    {
        if (!temp.empty())
        {
            infoText.push_back(temp);

            TextRenderInfo info;
            info.color = DirectX::SimpleMath::Color(1.f, 1.f, 1.f);
            info.font = Resources::Fonts::KG26; // ofc, KG26 everyone knows that
            info.text = infoText[infoText.size() - 1].c_str();
            info.position = DirectX::SimpleMath::Vector2(x, y);
            this->info.push_back(info);
        }

        y += 40;
    }

    // hp awfulness
    hpBar.color = DirectX::SimpleMath::Color{ 1.f, 0.2f, 0.2f };
    hpBar.font = Resources::Fonts::KG26;
    hpBar.position = DirectX::SimpleMath::Vector2(250.f, 640.f);
}

EnemyBossBaddie::~EnemyBossBaddie()
{
    RenderQueue::get().clearAllQueues(); // TEMPORARY SOLUTION, ISSUE: DELETING SOMEONE WITH POINTER TO GRAPHICS DATA AFTER THE QUEUEING!!!
}

/*

    THIS METHOD CREATES THE ABILITES; CHANGE THEM HERE!

*/
void EnemyBossBaddie::createAbilities()
{
    AbilityData data;

    /* ABILITY ONE */
    data.cooldown = 25000.f; // Ability One Data
    data.duration = 14500.f;
    data.randomChanche = 50;

    auto onUse = [&](Player& player, Ability &ability) -> void {
        Sound::NoiseMachine::Get().playSFX(Sound::SFX::BOSS_1_ABILITY_1, nullptr, true);
    };

    auto onTick = [&](Player& player, Ability &ability) -> void {
        btVector3 force = (getPositionBT() - player.getPositionBT()).normalize() *
            std::pow((1.f - (ability.getCurrentDuration() / ability.getData().duration)), 3) * ABILITY_1_MOD;
        player.getCharController()->applyImpulse(force);
    };

    abilities[AbilityId::ONE] = Ability(data, onTick, onUse);

    /* ABILITY TWO */

    data.cooldown = 13500.f; // Ability One Data
    data.duration = 0.f;
    data.randomChanche = 150;

    auto onUse1 = [&](Player& player, Ability &ability) -> void {
        Sound::NoiseMachine::Get().playSFX(Sound::SFX::BOSS_1_ABILITY_2, nullptr, true);

        for (int i = 0; i < NECRO_COUNT; i++)
        {
            btVector3 to = player.getPositionBT() - getPositionBT();
            float len = to.length();
            Projectile *pj = shoot((to + btVector3{ 25.f * i - 25.f, 90, 0 }).normalize(),
                Resources::Models::UnitCube, PROJECTILE_SPEED + (len * 0.5f), 2.5f, 0.6f);

            pj->addCallback(ON_COLLISION, [&](CallbackData &data) -> void {
                SpawnEnemy(EnemyType::NECROMANCER, data.caller->getPositionBT(), {});
            });
        }
    };

    auto onTick1 = [&](Player& player, Ability &ability) -> void {
    };

    abilities[AbilityId::TWO] = Ability(data, onTick1, onUse1);

    /* MELEE */

    data.cooldown = 7500.f; // Ability One Data
    data.duration = 4500.f;
    data.randomChanche = 0;

    auto onUse2 = [&](Player& player, Ability &ability) -> void {
        getSoundSource()->playSFX(Sound::SFX::BOSS_1_MELEE_USE);
    };

    auto onTick2 = [&](Player& player, Ability &ability) -> void {
        if (ability.getCurrentDuration() <= 0.f)
        {
            btVector3 to = player.getPositionBT() - getPositionBT();
            if (to.length() < MELEE_RANGE)
            {
                Sound::NoiseMachine::Get().playSFX(Sound::SFX::JUMP, nullptr, true);
                player.takeDamage(1, true); // shield charge wont save ya bitch
                player.getCharController()->applyImpulse(to.normalize() * MELEE_PUSHBACK); 
            }
        }
    };

    abilities[AbilityId::MELEE] = Ability(data, onTick2, onUse2);

    /* AB 3 */
    data.cooldown = 1000.f; // Ability One Data
    data.duration = 0.f;
    data.randomChanche = 0;

    auto onUse3 = [&](Player& player, Ability &ability) -> void {
        float m_sliceSize = (1.f * 3.14f) / 4.f;
        btVector3 dir = player.getPositionBT() - getPositionBT();
        for (int i = -1; i <= 3; i++) // todo def
        {
            dir += btVector3(cos(m_sliceSize * (i + RandomGenerator::singleton().getRandomFloat(-0.33f, 0.33f))),
                0.f, sin(m_sliceSize * (i + RandomGenerator::singleton().getRandomFloat(-0.33f, 0.33f))));
            shoot(dir.normalize(), Resources::Models::Files::SkySphere, PROJECTILE_SPEED, 0.f, 1.6f, true);
        }
    };

    auto onTick3 = [&](Player& player, Ability &ability) -> void {
        
    };

    abilities[AbilityId::THREE] = Ability(data, onTick3, onUse3);
}

void EnemyBossBaddie::useAbility(Player &target)
{
    if (!abilities[AbilityId::MELEE].isUsingAbility())
        abilities[AbilityId::MELEE].useAbility(target);
}

void EnemyBossBaddie::useAbility(Player &target, int phase)
{
    printf("Phase: %d\n", phase);
    switch (phase)
    {
        case 0:
            if (!abilities[AbilityId::ONE].useAbility(target))
            {
                abilities[AbilityId::TWO].useAbility(target);
            }
            break;
        case 1:
            abilities[AbilityId::THREE].useAbility(target);
            abilities[AbilityId::TWO].useAbility(target);
            break;
        case 2:
            break;
    }
}

void EnemyBossBaddie::onCollision(PhysicsObject &other, btVector3 contactPoint, float dmgMultiplier)
{
if (Player *e = dynamic_cast<Player*>(&other))
    {
    }
    else if (Projectile *pj = dynamic_cast<Projectile*> (&other))
    {
        if (!pj->getProjectileData().enemyBullet)
        {
            damage(static_cast<int> (pj->getProjectileData().damage * dmgMultiplier));

            if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                getStatusManager().addStatus(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME), true);
        }
    }
}

void EnemyBossBaddie::updateSpecific(Player &player, float deltaTime)
{
    // test
    for (auto &pair : abilities)
        pair.second.update(deltaTime, player);

    // HP BAR CHEAT
    hp = L"HP: ";
    for (int i = 0; i < static_cast<int>((static_cast<float> (getHealth()) / getMaxHealth()) * 10.f); i++)
        hp += L"I";
    hpBar.text = hp.c_str();

    RenderQueue::get().queue(&hpBar);
}

void EnemyBossBaddie::updateDead(float deltaTime)
{
    // this is for my own amusement, not intended to be in final product or testing :>
    for (auto &info : info)
        info.position.y -= deltaTime / 70.f;
    renderSpecific(); // testing testing testing
}

void EnemyBossBaddie::renderSpecific() const
{
    // this is only for fun
    for (auto &info : info)
        if (info.position.y < 750.f && info.position.y > -50.f)
            RenderQueue::get().queue(&info);
}
