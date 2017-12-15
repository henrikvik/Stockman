#include <AI\EnemyBossBaddie.h>
#include <AI\Trigger\TriggerTrapExplosive.h>
#include <Player\Player.h>

#include <Projectile\Projectile.h>
#include <Projectile\ProjectileStruct.h>

#include <Misc\Sound\NoiseMachine.h>
#include <Graphics\include\Structs.h>
#include <Misc\ComboMachine.h>

#include <sstream>
using namespace Logic;

#define AB4_BASESPEED 27.f // for const expr
#define AB4_PATTERNS 3
#define PI 3.14f

#define NECRO_COUNT 3

const float EnemyBossBaddie::PROJECTILE_SPEED = 25.0f,
            EnemyBossBaddie::ABILITY_1_MOD = 0.6f,
            EnemyBossBaddie::TOTAL_HP_BAR = 500.f,
            EnemyBossBaddie::PROJECTILE_SCALE = 7.5f;

const int   EnemyBossBaddie::BASE_DAMAGE = 1,
            EnemyBossBaddie::MAX_HP = 169690, // noise
            EnemyBossBaddie::SCORE = 150000,
            EnemyBossBaddie::INDICATORS = 12; // Big guy, for you. well memed // Big guy, for you. well memed // Big guy, for you. well memed


// melee var
const float EnemyBossBaddie::MELEE_RANGE = 30.f,
            EnemyBossBaddie::MELEE_PUSHBACK = 0.1f;
// speed var
const float EnemyBossBaddie::BASE_SPEED_P1 = 13.f,
            EnemyBossBaddie::BASE_SPEED_P2 = 16.f,
            EnemyBossBaddie::BASE_SPEED_P3 = 20.f;
// extra var
const float EnemyBossBaddie::BASE_WEAKNESS = 0.75f,
            EnemyBossBaddie::MELEE_WEAKNESS = 2.f,
            EnemyBossBaddie::STUNNED_WEAKNESS = 4.5f;
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
    : Enemy(Resources::Models::Grunt, body,
        halfExtent, MAX_HP, BASE_DAMAGE, BASE_SPEED_P1, EnemyType::BOSS_1, 0, { 0.55f, -3.5f, -0.2f }),
    hpBarOutline(400.0f, 70.0f, TOTAL_HP_BAR, 25.0f, Resources::Textures::Gamesheet, FloatRect({0.0459f, 0.87012f}, {0.95508f, 0.95801f})),
    hpBar(400.0f, 70.0f, TOTAL_HP_BAR, 25.0f, Resources::Textures::Gamesheet, FloatRect({0.04688f, 0.76855f}, {0.9541f, 0.85645f}))
{
    setBehavior(BOSS_BADDIE);

    ab4Speed = AB4_BASESPEED;
    ab4Pattern = 0;
    ab4PatternDir = true;

    Sound::NoiseMachine::Get().stopGroup(Sound::CHANNEL_GROUP::CHANNEL_MUSIC);
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::BOSS_1_MUSIC_1, nullptr, true);
    createAbilities();
    getAnimatedModel().set_next("Run_Grunt");
    
    addCallback(ON_DEATH, [&](CallbackData &data)
    {
        ComboMachine::Get().kill(SCORE);
        Sound::NoiseMachine::Get().stopGroup(Sound::CHANNEL_GROUP::CHANNEL_MUSIC);
        Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::BOSS_1_MUSIC_2, nullptr, true);

        for (Projectile *pj : meleeIndicators) {
            if (pj) {
                pj->setDead(true);
            }
        }
    });

    light.color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f);
    light.intensity = 0.8f;
    light.range = 10.0f;

    fancyAF.color = DirectX::Colors::White;
    fancyAF.scale = 1.f;
    fancyAF.text = L"";

    lolSpeech = false;
    speechTime = 15000.f;
    stunned = false;

    meleeIndicators.resize(INDICATORS);
}

EnemyBossBaddie::~EnemyBossBaddie()
{
    Sound::NoiseMachine::Get().stopAllGroups();
    RenderQueue::get().clearAllQueues(); // TEMPORARY SOLUTION, ISSUE: DELETING SOMEONE WITH POINTER TO GRAPHICS DATA AFTER THE QUEUEING!!!
}

/*

    THIS METHOD CREATES THE ABILITES; CHANGE THEM HERE!

*/
void EnemyBossBaddie::createAbilities()
{
    static RandomGenerator &gen = RandomGenerator::singleton();
    static Graphics::ParticleEffect bossTrail = Graphics::FXSystem->getEffect("DamageProjTrail");
    AbilityData data;

    nicePjData.effect = bossTrail;
    nicePjData.hasEffect = true;
    nicePjData.effectVelocity = false;
    nicePjData.effectActivated = true;

    /* ABILITY ONE */
    data.cooldown = 17500.f;
    data.duration = 4000.f;
    data.randomChanche = 40;

    auto onUse = [&](Player& player, Ability &ability) -> void {
        fancyAF.text = L"Apocalypse!";
        Sound::NoiseMachine::Get().playSFX(Sound::SFX::BOSS_1_ABILITY_1, nullptr, true);
    };

    auto onTick = [&](Player& player, Ability &ability) -> void {
        if (ability.getCurrentDuration() <= 0.f) {
            fancyAF.text = L"";
            Graphics::FXSystem->addEffect("DamageBoom", getPosition());
            getRigidBody()->getWorldTransform().setOrigin(player.getPositionBT() + btVector3(0.f, 100.f, 0.f));
        }
    };

    abilities[AbilityId::ONE] = Ability(data, onTick, onUse);

    /* ABILITY TWO */

    data.cooldown = 7500.f;
    data.duration = 0.f;
    data.randomChanche = 53;

    auto onUse1 = [&](Player& player, Ability &ability) -> void {
        Sound::NoiseMachine::Get().playSFX(Sound::SFX::BOSS_1_ABILITY_2, nullptr, true);

        for (int i = 0; i < NECRO_COUNT; i++)
        {
            btVector3 to = player.getPositionBT() - getPositionBT();
            float len = to.length();
            Projectile *pj = shoot((to + btVector3{ 25.f * i - 25.f, 90, 0 }).normalize(),
                nicePjData, PROJECTILE_SPEED + (len * 0.4f), 2.5f, 0.6f, true);

            pj->addCallback(ON_DESTROY, [&](CallbackData &data) -> void {
                if (gen.getRandomInt(0, 5) < 2) {
                    SpawnEnemy(EnemyType::NECROMANCER, data.caller->getPositionBT(), {});
                }
                else 
                {
                    SpawnTrigger(static_cast<int> (Trigger::TriggerType::TRAP_EXPLOSIVE),
                        data.caller->getPositionBT() + btVector3(0.f, 2.3f, 0.f),
                        {}
                    );
                }
            });
        }
    };

    auto onTick1 = [&](Player& player, Ability &ability) -> void {
    };

    abilities[AbilityId::TWO] = Ability(data, onTick1, onUse1);

    /* MELEE */

    data.cooldown = 7500.f;
    data.duration = 2650.f;
    data.randomChanche = 0;

    indicatorData.damage = 0;
    indicatorData.scale = 1.f;
    indicatorData.enemyBullet = true;
    indicatorData.isSensor = true;
    indicatorData.speed = 0;
    indicatorData.ttl = data.duration * 0.95f;
    indicatorData.meshID = Resources::Models::Bone;
    indicatorData.type = ProjectileTypeDefenderShield;
    indicatorData.shouldRender = true;

    auto onUse2 = [&](Player& player, Ability &ability) -> void {
        getSoundSource()->playSFX(Sound::SFX::BOSS_1_MELEE_USE);
        fancyAF.text = L"Now you face your tragic end!";
        getAnimatedModel().set_next("Attack_Grunt", [&]() -> void {
            setMoveSpeed(0.f);
            getAnimatedModel().set_delta_multiplier(getAnimatedModel().get_animation_time() * 1000.f / (ability.getCurrentDuration()) - 0.1f);
            getAnimatedModel().set_next("Run_Grunt", [&]() -> void {
                getAnimatedModel().set_delta_multiplier(1.f);
            });
        });

        for (int i = 0; i < INDICATORS; i++) {
            Projectile *pj = SpawnProjectile(indicatorData, btVector3(0, 0, 0), btVector3(0, 0, 0), *this);
            meleeIndicators[i] = pj;
            if (pj) {
                increaseCallbackEntities();
                pj->addCallback(ON_DESTROY, [=](CallbackData &data) {
                    meleeIndicators[i] = nullptr;
                    decreaseCallbackEntities();
                });
            }
        }
    };

    auto onTick2 = [&](Player& player, Ability &ability) -> void {
        constexpr float piece = 3.14f / INDICATORS * 2;

        for (int i = 0; i < meleeIndicators.size(); i++) {
            if (meleeIndicators[i]) {
                float scl = 1.f - (ability.getCurrentDuration() / ability.getData().duration);
                meleeIndicators[i]->getRigidBody()->getWorldTransform().setOrigin(
                    getPositionBT() + btVector3(
                        std::cos(i * piece) * MELEE_RANGE * scl,
                        0.0f,
                        std::sin(i * piece) * MELEE_RANGE * scl
                    )
                );
            }
        }

        if (ability.getCurrentDuration() <= 0.f)
        {
            fancyAF.text = L"";
            btVector3 to = player.getPositionBT() - getPositionBT();
            if (to.length() < MELEE_RANGE)
            {
                Sound::NoiseMachine::Get().playSFX(Sound::SFX::JUMP, nullptr, true);
                player.takeDamage(getBaseDamage(), true); // shield charge wont save ya
                to.setY(250.f);
                player.getCharController()->applyImpulse(to.normalized() * MELEE_PUSHBACK); 
            }

            for (Projectile *pj : meleeIndicators) {
                if (pj) {
                    pj->setDead(true);
                }
            }
        }
    };

    abilities[AbilityId::MELEE] = Ability(data, onTick2, onUse2);

    /* AB 3 */
    data.cooldown = 3000.f;
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
            shoot(dir.normalize(), nicePjData, PROJECTILE_SPEED, 0.f, PROJECTILE_SCALE, true);
        }
    };

    auto onTick3 = [&](Player& player, Ability &ability) -> void {
        
    };

    abilities[AbilityId::THREE] = Ability(data, onTick3, onUse3);

    /*
    
        PHASE 3
    
    */

    /* AB 4 */
    data.cooldown = 3000.f;
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
        static float len = 100.f;

        Sound::NoiseMachine::Get().playSFX(Sound::SFX::BOSS_1_ABILITY_5, nullptr, true);

        btVector3 playerPos = player.getPositionBT();
        btVector3 temp;
        int skip = RandomGenerator::singleton().getRandomInt(0, 9);

        ProjectileData data = nicePjData;
        data.damage = 1;
        data.mass = 1.f;
        data.scale = PROJECTILE_SCALE;
        data.enemyBullet = data.isSensor = true;
        data.speed = 19.f;
        data.ttl = len * 100.f;
        data.gravityModifier = 0;
        data.shouldRender = false;

        for (int i = 0; i < 10; i++)
        {
            if (i == skip) continue;
            
            temp = playerPos + btVector3(cos(slice * i), 0.f, sin(slice * i)) * len;
            Projectile *pj = SpawnProjectile(data, temp, (playerPos - temp).normalized(), *this);
            if (pj) {
                pj->getRigidBody()->setRestitution(btScalar(20.f));
                pj->getStatusManager().addUpgrade(StatusManager::BOUNCE);
            }
        }
    };

    auto onTick5 = [&](Player& player, Ability &ability) -> void {

    };

    abilities[AbilityId::FIVE] = Ability(data, onTick5, onUse5);
}

void EnemyBossBaddie::shootAbility4(Player const &player, int pattern, float speed)
{
    constexpr float rad = 3.14f;

    btVector3 dir = player.getPositionBT() - getPositionBT();
    dir.setY(0.f);
    Projectile *pj = nullptr;

    switch (pattern)
    {
    case 0:
        for (int i = -1; i <= 1; i++)
            pj = shoot(dir.normalize() + (btVector3(cos(rad), 0.f, sin(rad)) * (i * 0.35f)), nicePjData, speed, 0.f, PROJECTILE_SCALE, true);
        break;
    case 1:
        for (int i = 2; i < 4; i++)
            pj = shoot(dir.normalize() + (btVector3(cos(rad), 0.f, sin(rad)) * (i * 0.22f)), nicePjData, speed, 0.f, PROJECTILE_SCALE, true);
        for (int i = 2; i < 4; i++)
            pj = shoot(dir.normalize() + (btVector3(cos(rad), 0.f, sin(rad)) * (i * -0.22f)), nicePjData, speed, 0.f, PROJECTILE_SCALE, true);
        break;
    case 2:
        for (int i = -1; i <= 1; i++)
            pj = shoot(dir.normalize() + (btVector3(cos(rad), 0.f, sin(rad)) * (i * 0.08f)), nicePjData, speed, 0.f, PROJECTILE_SCALE, true);
        break;
    }

    if (pj)
        pj->getRigidBody()->setRestitution(btScalar(20.f));
}

void EnemyBossBaddie::damage(int damage)
{
    float mod = 0.f;

    if (stunned) mod = STUNNED_WEAKNESS;
    else if (abilities[AbilityId::MELEE].isUsingAbility()) mod = MELEE_WEAKNESS;
    else mod = BASE_WEAKNESS;

    float damageCalc = std::floor(damage * mod) - 5;
    if (damageCalc > 0) {
        Enemy::damage(damageCalc);

        SpawnDamageText(damageCalc, DirectX::Colors::FloralWhite);
    }
}

void EnemyBossBaddie::useAbility(Player &target)
{
    if (!abilities[AbilityId::ONE].isUsingAbility())
    {
        abilities[AbilityId::MELEE].useAbility(target);
    }
}

void EnemyBossBaddie::useAbility(Player &target, int phase)
{
    abilities[AbilityId::ONE].useAbility(target);
    switch (phase)
    {
        case 0:
            abilities[AbilityId::TWO].useAbility(target);
            if (!abilities[AbilityId::MELEE].isUsingAbility())
                setMoveSpeed(BASE_SPEED_P1);
            break;
        case 1:
            abilities[AbilityId::THREE].useAbility(target);
            abilities[AbilityId::TWO].useAbility(target);
            if (!abilities[AbilityId::MELEE].isUsingAbility())
                setMoveSpeed(BASE_SPEED_P2);
            break;
        case 2:
            abilities[AbilityId::FOUR].useAbility(target);
            abilities[AbilityId::FIVE].useAbility(target);
            if (!abilities[AbilityId::MELEE].isUsingAbility())
                setMoveSpeed(BASE_SPEED_P3);
            break;
    }
}

void EnemyBossBaddie::onCollision(PhysicsObject &other, btVector3 contactPoint, float dmgMultiplier)
{
    if (Projectile *pj = dynamic_cast<Projectile*> (&other))
    {
        if (!pj->getProjectileData().enemyBullet)
        {
            damage(static_cast<int> (pj->getProjectileData().damage * dmgMultiplier));

            getStatusManager().removeAllStatus(StatusManager::STUN);
            if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME));
        }
    }
}

void EnemyBossBaddie::updateSpecific(Player &player, float deltaTime)
{
    for (auto &pair : abilities)
        pair.second.update(deltaTime, player);

    //hp bar
    float healthleft = float(getHealth()) / float(getMaxHealth());
    hpBar.setScreenPos(Sprite::Points::TOP_LEFT, Sprite::Points::TOP_LEFT, 400.f, 70.f, healthleft * TOTAL_HP_BAR, 25.f);

    // prevent falling off
    if (getPositionBT().getY() < 0.5f) {
        getRigidBody()->setGravity({ 0.f, 9.82f * 25.f, 0.f });
    }
    else {
        getRigidBody()->setGravity({ 0.f, -9.82f * 10.f, 0.f });
    }

    // :D
    fancyAF.position = getPosition() + DirectX::SimpleMath::Vector3(2.f, 3.f, 2.f);

    if (getHealth() < MAX_HP * 0.35f) {
        lolSpeech = true;
   }
    
    if (lolSpeech && speechTime > 0.f) {
        speechTime -= deltaTime;

        // this is stupid but fun
        if (speechTime > 12000.f) {
            fancyAF.text = L"No questions remain unanswered. No doubts linger.";
        }  else if (speechTime > 9000.f) {
            fancyAF.text = L"You are this island's greatest champion!";
        } else if (speechTime > 7000.f) {
            fancyAF.text = L"You overcame every challenge I laid before you.";
        } else if (speechTime > 4500.f) {
            fancyAF.text = L"My mightiest servants have fallen before your relentless onslaught";
        } else {
            fancyAF.text = L"your unbridled fury... Is it truly righteousness that drives you?";
        }

        if (speechTime < 0.f) fancyAF.text = L"";
    }
}

void EnemyBossBaddie::updateDead(float deltaTime)
{

}

void EnemyBossBaddie::renderSpecific() const
{
    if (!fancyAF.text.empty()) {
        QueueRender(fancyAF);
    }
    hpBarOutline.render();
    hpBar.render();
}
