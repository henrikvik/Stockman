#include <AI\EnemyBossBaddie.h>
#include <Player\Player.h>

#include <Projectile\Projectile.h>
#include <Projectile\ProjectileStruct.h>

#include <Misc\Sound\NoiseMachine.h>
#include <Graphics\include\Structs.h>

#include <sstream>
using namespace Logic;

#define AB4_BASESPEED 27.f // for const expr
#define AB4_PATTERNS 3
#define PI 3.14f

#define NECRO_COUNT 3

const float EnemyBossBaddie::BASE_SPEED = 21.5f, EnemyBossBaddie::PROJECTILE_SPEED = 35.f,
            EnemyBossBaddie::ABILITY_1_MOD = 0.075f, EnemyBossBaddie::MELEE_RANGE = 27.5f,
            EnemyBossBaddie::MELEE_PUSHBACK = 0.3f;
const int EnemyBossBaddie::BASE_DAMAGE = 1, EnemyBossBaddie::MAX_HP = 35000; // Big guy, for you

/*
    @author Lukas Westling

    General Todo:
        Fix all the magic numbers (alot of them)
        Better math and cleaner functions
        Better code structure

    Don't do any of this if the boss is going to get removed / replaced.
    This is only a fun weekend project that might stay in the game.
*/

EnemyBossBaddie::EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent)
    : Enemy(Resources::Models::UnitCube, body,
        halfExtent, MAX_HP, BASE_DAMAGE, BASE_SPEED, EnemyType::BOSS_1, 0)
{
    setBehavior(BOSS_BADDIE);

    ab4Speed = AB4_BASESPEED;
    ab4Pattern = 0;
    ab4PatternDir = true;

    Sound::NoiseMachine::Get().stopAllGroups();
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::BOSS_1_MUSIC_1, nullptr, true);
    createAbilities();
    
    addCallback(ON_DEATH, [&](CallbackData &data)
    {
        Sound::NoiseMachine::Get().stopAllGroups();
        Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::BOSS_1_MUSIC_2, nullptr, true);
    });

    forFunTesting();

    light.color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f);
    light.intensity = 0.8f;
    light.range = 10.0f;
}

EnemyBossBaddie::~EnemyBossBaddie()
{
    Sound::NoiseMachine::Get().stopAllGroups();
    RenderQueue::get().clearAllQueues(); // TEMPORARY SOLUTION, ISSUE: DELETING SOMEONE WITH POINTER TO GRAPHICS DATA AFTER THE QUEUEING!!!
}

void EnemyBossBaddie::forFunTesting()
{
    std::wstring test = L"Created By: Stockman Games Entertainment\n\nProgrammers:\nAndreas Henriksson\nHenrik Vik\nJakob Nyberg\nSimon Fredholm\nLukas Westling\nEmanuel Bjurman\nFelix Kaaman\nJohan Ottosson \n Simon Sandberg\n\nTechnical Artists: Johan Ottosson \n  Simon Sandberg\n\nThe Bad Format: .lw\n\nGitmeister: Henrik Vik\n\nInvestor: Gabe Newell\n\nThe guy that fixed everyone elses stuff: Henrik Vik\n\nGame Manager & Producer: Henrik Vik\n\nWonderful Music: Banana\n\n\nThank you\nfor playing!\n\n\n\n\n\n\n\n\n\n\n\n\nEnemies:\nNecromancer (The annoying dude)\nNecromancer Minion (The dude everyone wants to nerf)\nThe Boss (The boss everyone hates)\nTorpedo Ted\nReznor, The Secret boss.\nLarry Koopa\nLemmy Koopa\nMorton Koopa Jr.\nThe Lich King\nDoctor Boom\nGrim Patron\nThe Hogger\nHanzo Mains\nVampires from Castle Wars\nOP Ferie Dragons\nKalphite Queen\nDr Stockman\nMr King Dice\nRace Conditions\nDeadlines\nPlaytesting\nStandup meetings\nMagic numbers\nHealthbars\nBowser Baloon Minigame from mario party 4 and 3\nWow: Classic Servers\nChimaeron\nMitch McConnell (turtle guy)\nIce Poseidon\nMr Garrison (President Of The United States Of America)\nProfessor Chaos\nKyle Brofloski\nStan Marsh\nRandy Marsh\nHeidi Turner :(\nPasha Biceps\nOlofmeister\nFriberg\nMoonmoon\nTwitch ads every fucking second on the yugioh stream\nH3h3productions\nKennyS\nNaniwa\n\n\nGot this far without cheats or crashes? nice.\nSpecial Thanks to: Henrik Vik!\n\n\n\nStockman studio is not responsible for any crashes or bugs that might damage your computer."; // See it in game not here dude :>
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

/*

    THIS METHOD CREATES THE ABILITES; CHANGE THEM HERE!

*/
void EnemyBossBaddie::createAbilities()
{
    AbilityData data;

    /* ABILITY ONE */
    data.cooldown = 25000.f;
    data.duration = 14500.f;
    data.randomChanche = 50;

    auto onUse = [&](Player& player, Ability &ability) -> void {
        Sound::NoiseMachine::Get().playSFX(Sound::SFX::BOSS_1_ABILITY_1, nullptr, true);
    };

    auto onTick = [&](Player& player, Ability &ability) -> void {
        /*
        btVector3 force = (getPositionBT() - player.getPositionBT()).normalize() *
            std::pow((1.f - (ability.getCurrentDuration() / ability.getData().duration)), 3) * ABILITY_1_MOD;
        player.getCharController()->applyImpulse(force); */
    };

    abilities[AbilityId::ONE] = Ability(data, onTick, onUse);

    /* ABILITY TWO */

    data.cooldown = 15000.f;
    data.duration = 0.f;
    data.randomChanche = 200;

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

    data.cooldown = 7500.f;
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
    data.cooldown = 1400.f;
    data.duration = 0.f;
    data.randomChanche = 0;

    auto onUse3 = [&](Player& player, Ability &ability) -> void {
        constexpr float m_sliceSize = PI / 4.f;

        btVector3 dir = player.getPositionBT() - getPositionBT();
        dir.setY(-0.0012f);

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

    /*
    
        PHASE 3
    
    */

    /* AB 4 */
    data.cooldown = 950.f;
    data.duration = 0.f;
    data.randomChanche = 0;

    auto onUse4 = [&](Player& player, Ability &ability) -> void {
        constexpr float SPEED_PIECE = AB4_BASESPEED / 22.f;
        ab4Speed += SPEED_PIECE; // faster, stronger

        // handle the patterns
        if (ab4PatternDir)
        {
            ab4Pattern++;
            if (ab4Pattern >= AB4_PATTERNS - 1) ab4PatternDir = false;
        }
        else
        {
            ab4Pattern--;
            if (ab4Pattern <= 0) ab4PatternDir = true;
        }

        shootAbility4(
            player,
            ab4Pattern, 
            ab4Speed 
        );
    };

    auto onTick4 = [&](Player& player, Ability &ability) -> void {

    };

    abilities[AbilityId::FOUR] = Ability(data, onTick4, onUse4);

    /* AB 5 So many magic numbers, sorry (TODO) */
    data.cooldown = 7550.f;
    data.duration = 0.f;
    data.randomChanche = 25;

    auto onUse5 = [&](Player& player, Ability &ability) -> void {
        constexpr float slice = PI * 2.f / 10.f;
        static float len = 60.f;

        Sound::NoiseMachine::Get().playSFX(Sound::SFX::BOSS_1_ABILITY_5, nullptr, true);

        btVector3 playerPos = player.getPositionBT();
        btVector3 temp;
        int skip = RandomGenerator::singleton().getRandomInt(0, 9);

        ProjectileData data;
        data.damage = 1;
        data.mass = 1.f;
        data.scale = 1.5f;
        data.enemyBullet = data.isSensor = true;
        data.meshID = Resources::Models::Files::SkySphere;
        data.speed = 19.f;
        data.ttl = len * 60.f;
        data.gravityModifier = 0;
        data.shouldRender = false;

        for (int i = 0; i < 10; i++)
        {
            if (i == skip) continue;
            
            temp = playerPos + btVector3(cos(slice * i), 0.f, sin(slice * i)) * len;
            SpawnProjectile(data, temp, (playerPos - temp).normalize(), *this);
        }
    };

    auto onTick5 = [&](Player& player, Ability &ability) -> void {

    };

    abilities[AbilityId::FIVE] = Ability(data, onTick5, onUse5);
}

void EnemyBossBaddie::shootAbility4(Player const &player, int pattern, float speed)
{
    constexpr float rad = 3.14f;
    constexpr Resources::Models::Files model = Resources::Models::Files::SkySphere;

    btVector3 dir = player.getPositionBT() - getPositionBT();
    dir.setY(0.f);

    switch (pattern)
    {
    case 0:
        for (int i = -1; i <= 1; i++)
            shoot(dir.normalize() + (btVector3(cos(rad), 0.f, sin(rad)) * (i * 0.35f)), model, speed, 0.f, 2.f, true);
        break;
    case 1:
        for (int i = 2; i < 4; i++)
            shoot(dir.normalize() + (btVector3(cos(rad), 0.f, sin(rad)) * (i * 0.22f)), model, speed, 0.f, 2.9f, true);
        for (int i = 2; i < 4; i++)
            shoot(dir.normalize() + (btVector3(cos(rad), 0.f, sin(rad)) * (i * -0.22f)), model, speed, 0.f, 2.9f, true);
        break;
    case 2:
        for (int i = -1; i <= 1; i++)
            shoot(dir.normalize() + (btVector3(cos(rad), 0.f, sin(rad)) * (i * 0.08f)), model, speed, 0.f, 2.4f, true);
        break;
    }
}

void EnemyBossBaddie::damage(int damage)
{
    if (damage > 5)
        Enemy::damage(damage - 5); // make buff
}

void EnemyBossBaddie::useAbility(Player &target)
{
    if (!abilities[AbilityId::ONE].isUsingAbility())
        abilities[AbilityId::MELEE].useAbility(target);
}

void EnemyBossBaddie::useAbility(Player &target, int phase)
{
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
            abilities[AbilityId::FOUR].useAbility(target);
            abilities[AbilityId::FIVE].useAbility(target);
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
                getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME));
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
    int parts = static_cast<int>((static_cast<float> (getHealth()) / getMaxHealth()) * 10.f);
    for (int i = 0; i < parts; i++)
        hp += L"I";
    hp += L" " + std::to_wstring(parts) + L" / 10";
    hpBar.text = hp.c_str();
}

void EnemyBossBaddie::updateDead(float deltaTime)
{
    // this is for my own amusement, not intended to be in final product or testing :>
    for (auto &info : info)
        info.position.y -= deltaTime / 70.f;

    // this is only for fun
    for (auto &info : info)
        if (info.position.y < 750.f && info.position.y > -50.f)
            QueueRender(info);
}

void EnemyBossBaddie::renderSpecific() const
{
    QueueRender(hpBar);
}
