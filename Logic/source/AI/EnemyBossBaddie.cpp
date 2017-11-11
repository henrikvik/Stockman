#include <AI\EnemyBossBaddie.h>

#include <Misc\Sound\NoiseMachine.h>
#include <Player\Player.h>
#include <Projectile\Projectile.h>
#include <sstream>

#include <Graphics\include\Structs.h>
using namespace Logic;

const float EnemyBossBaddie::BASE_SPEED = 1.5f, EnemyBossBaddie::PROJECTILE_SPEED = 35.f,
            EnemyBossBaddie::ABILITY_1_MOD = 0.05f;
const int EnemyBossBaddie::BASE_DAMAGE = 1, EnemyBossBaddie::MAX_HP = 100; // Big guy, for you
#define NECRO_COUNT 2

EnemyBossBaddie::EnemyBossBaddie(btRigidBody* body, btVector3 &halfExtent)
    : Enemy(Resources::Models::UnitCube, body,
        halfExtent, MAX_HP, BASE_DAMAGE, BASE_SPEED, EnemyType::BOSS_1, 0)
{
    setBehavior(BOSS_BADDIE);

    Sound::NoiseMachine::Get().stopAllGroups();
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::BOSS_1_MUSIC_1, nullptr, true);
    createAbilities();

    addCallback(ON_DEATH, [&](CallbackData &data)
    {
        Sound::NoiseMachine::Get().stopAllGroups();
        Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::BOSS_1_MUSIC_2, nullptr, true);
    });

    // test
    std::wstring test = L"Created By: Stockman Games Entertainment\n\nProgrammers:\nAndreas Henriksson\nHenrik Vik\nJakob Nyberg\nSimon Fredholm\nLukas Westling\nEmanuel Bjurman\nFelix Kaaman\nJohan\nSimon\n\n\nGraphics: Johan & Simon\n\nTechnical Artists: Johan & Simon\n\nThe Bad Format: .lw\n\nGitmeister: Henrik Vik\n\nThe guy that fixed everyone elses stuff: Henrik Vik\n\nGame Manager & Producer: Henrik Vik\n\nWonderful Music: Banana\n\n\nThank you\nfor playing!\n\n\n\n\n\n\n\n\n\n\nEnemies:\nNecromancer (The annoying dude)\nNecromancer Minion (The dude everyone wants to nerf)\nThe Boss (The boss everyone hates)\nThe Secret boss no one have found\n\nYou still reading this?\nYou can leave now\nPlay a good game\nA Game where the AI doesn't crash all the time\nBye!\nSpecial Thanks to: Henrik Vik!\n\nStockman studio is not responsible for any crashes or bugs that might damage your computer."; // See it in game not here dude :>
    std::wstringstream str(test);
    std::wstring temp;
    float x = 300.f, y = 715.5f;
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
            std::pow((1.01f - (ability.getCurrentDuration() / ability.getData().duration)), 3) * ABILITY_1_MOD;
        player.getCharController()->applyImpulse(force);
    };

    abilities[AbilityId::ONE] = Ability(data, onTick, onUse);

    /* ABILITY TWO */

    data.cooldown = 16500.f; // Ability One Data
    data.duration = 0.f;
    data.randomChanche = 200;

    auto onUse2 = [&](Player& player, Ability &ability) -> void {
        for (int i = 0; i < NECRO_COUNT; i++)
        {
            Projectile *pj = shoot(((player.getPositionBT() - getPositionBT()) + btVector3{ 60.f * i - 30.f, 80, 0 }).normalize(),
                Resources::Models::UnitCube, PROJECTILE_SPEED, 2.5f, 0.6f);

            pj->addCallback(ON_COLLISION, [&](CallbackData &data) -> void {
                SpawnEnemy(EnemyType::NECROMANCER, data.caller->getPositionBT(), {});
            });
        }
    };

    auto onTick2 = [&](Player& player, Ability &ability) -> void {
    };

    abilities[AbilityId::TWO] = Ability(data, onTick2, onUse2);
}

void EnemyBossBaddie::useAbility(Player & target)
{
    // todo melee
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
            break;
        case 2:
            break;
    }
}

void EnemyBossBaddie::onCollision(PhysicsObject &other, btVector3 contactPoint, float dmgMultiplier)
{
    if (Enemy *e = dynamic_cast<Enemy*>(&other))
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
    useAbility(player, 0);
    for (auto &pair : abilities)
        pair.second.update(deltaTime, player);
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
