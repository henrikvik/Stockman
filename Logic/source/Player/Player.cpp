#include "Player/Player.h"

#include <DebugDefines.h>
#include <Misc\ComboMachine.h>

#include <AI\EnemyTest.h>
#include <AI\Trigger\Trigger.h>

#include <Projectile\ProjectileManager.h>

#include <Player\Weapon\WeaponManager.h>
#include <Player\Weapon\Weapon.h>
#include <Player\Weapon\AmmoContainer.h>

#include <Player\Skill\SkillManager.h>
#include <Player\Skill\Skill.h>

#include <Misc\Sound\NoiseStructs.h>
#include <Physics\Physics.h>
#include <Projectile\Projectile.h>

#include <Singletons\Profiler.h>
#include <Singletons\DebugWindow.h>
#include <Graphics\include\MainCamera.h>
#include <Graphics\include\Device.h>

#include <Engine/Settings.h>
#include <Engine/Engine.h>

using namespace Logic;

#define PLAYER_MOVEMENT_ACCELERATION	0.0002f
#define PLAYER_MOVEMENT_AIRACCELERATION	0.005f
#define PLAYER_MOVEMENT_AIRSTRAFE_SPEED 0.004f
#define PLAYER_SPEED_LIMIT				0.04f
#define PLAYER_STRAFE_ANGLE				0.92f
#define PLAYER_FRICTION					20.f
#define PLAYER_FRICTION_MIN             0.1f
#define PLAYER_AIR_FRICTION				1.f
#define PLAYER_JUMP_SPEED				0.008f
#define PLAYER_MAX_MOVE                 30.f

const int Player::MIN_Y = -80, Player::MAX_HP = 3;
btVector3 Player::startPosition = btVector3(0.f, 6.f, 0.f);

Player::Player(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent)
    : Entity(body, halfExtent),
      m_damageTintTimer(0.f),
      m_upgradeTintTimer(0.f),
      m_pickupTintTimer(0.f)
{
    m_weaponManager = newd WeaponManager();
    m_skillManager = newd SkillManager();
    m_listenerData = newd Sound::ListenerData();
    currentSkills[0] = -1;
    currentSkills[1] = -1;
}

Player::~Player()
{
	clear();
}

void Player::init(Physics* physics, ProjectileManager* projectileManager)
{
    Settings& setting = Settings::getInstance();
	m_weaponManager->init(projectileManager);
	m_skillManager->init(physics, projectileManager);
	m_physPtr = physics;

    // Add default fire dmg
    getStatusManager().addUpgrade(StatusManager::FIRE_UPGRADE);

	btCapsuleShape* playerShape = new btCapsuleShape(PLAYER_SIZE_RADIUS, PLAYER_SIZE_HEIGHT);
	btPairCachingGhostObject* ghostObject = m_physPtr->createPlayer(playerShape, startPosition);
	ghostObject->setUserPointer(this);

	m_charController = new btKinematicCharacterController(ghostObject, playerShape, 0.2f, btVector3(0.f, 1.f, 0.f));
	m_charController->setGravity({ 0.f, -PLAYER_GRAVITY, 0.f });
    m_charController->setLinearVelocity({ 0.f, 0.f, 0.f });
    m_charController->setFallSpeed(1.f);
	m_physPtr->addAction(m_charController);
    m_charController->warp(startPosition);
    m_charController->jump({ 0.f, PLAYER_JUMP_SPEED, 0.f });

	// Stats
	m_hp = PLAYER_STARTING_HP;
    currentWeapon = 0;

	// Default mouse sensetivity, lookAt
	m_camYaw = 90;
	m_camPitch = 5;

	m_playerState = PlayerState::STANDING;

    m_godMode = m_noclip = false;

    registerDebugCmds();

    m_lastPos = getPositionBT();
	m_forward = DirectX::SimpleMath::Vector3(0, 0, 1);
	m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED;
	m_moveDir.setZero();
	m_moveSpeed = 0.f;
    m_moveSpeedMod = 1.0f;
    m_permanentSpeedMod = 1.0f;
    m_jumpSpeedMod = 1.0f;
	m_acceleration = PLAYER_MOVEMENT_ACCELERATION;
	m_deacceleration = m_acceleration * 0.5f;
	m_airAcceleration = PLAYER_MOVEMENT_AIRACCELERATION;
	m_jumpSpeed = PLAYER_JUMP_SPEED;
	m_wishDir.setZero();
	m_wishDirForward = 0.f;
	m_wishDirRight = 0.f;
	m_wishJump = false;
    m_firstJump = true;

    m_wasInAir = false;

	// Default controlls
	m_moveLeft = DirectX::Keyboard::Keys::A;
	m_moveRight = DirectX::Keyboard::Keys::D;
	m_moveForward = DirectX::Keyboard::Keys::W;
	m_moveBack = DirectX::Keyboard::Keys::S;
	m_jump = DirectX::Keyboard::Keys::Space;
	m_switchWeaponOne = DirectX::Keyboard::Keys::D1;
	m_switchWeaponTwo = DirectX::Keyboard::Keys::D2;
	m_switchWeaponThree = DirectX::Keyboard::Keys::D3;
	m_reloadWeapon = DirectX::Keyboard::Keys::R;
    m_useSkillPrimary = DirectX::Keyboard::Keys::Q;
    m_useSkillSecondary = DirectX::Keyboard::Keys::E;
    m_useSkillTertiary = DirectX::Keyboard::Keys::F;
	m_listenerData->update({ 0, 0, 0 }, { 0, 1, 0 }, { m_forward.x, m_forward.y, m_forward.z }, m_charController->getGhostObject()->getWorldTransform().getOrigin());

    m_stunned = false;
    resetTargeted();
}

void Player::registerDebugCmds()
{
    DebugWindow *win = DebugWindow::getInstance();
    win->registerCommand("LOG_SET_MOUSE_SENSITIVITY", [&](std::vector<std::string> &para) -> std::string {
        try
        { // Boilerplate code bois
            Settings::getInstance().setMouseSense(stof(para[0]));
        }
        catch (int)
        {
            return "That is not going to work";
        }
        return "Mouse sens set";
    });

    win->registerCommand("LOG_GODMODE", [&](std::vector<std::string> &para) -> std::string {
        m_godMode = !m_godMode;
        return "Godmode updated";
    });

    win->registerCommand("LOG_NOCLIP", [&](std::vector<std::string> &para) -> std::string {
        m_noclip = !m_noclip;
        if (m_noclip)
            m_charController->setGravity({ 0.f, 0.f, 0.f }); // remove gravity
        else {
            m_charController->setGravity({ 0.f, -PLAYER_GRAVITY, 0.f });
            // reset movement
            m_moveDir.setZero();
            m_moveSpeed = 0.f;
        }
        return "Noclip updated";
    });

    win->registerCommand("LOG_PLAYER_STUN", [&](std::vector<std::string> &args) -> std::string
    {
        getStatusManager().addStatusResetDuration(StatusManager::STUN, 1);

        return "Player is stunned";
    });

    win->registerCommand("LOG_ADD_UPGRADE", [&](std::vector<std::string> &args) -> std::string
    {
        try
        {
            int stacks = 1;
            if (args.size() > 1)
                stacks = std::stoi(args[1]);
            upgrade(static_cast<StatusManager::UPGRADE_ID> (std::stoi(args[0])), stacks);
            return "Upgrade added";
        }
        catch (std::exception const &ex)
        {
            return "I have the highground DLC";
        }
    });

    win->registerCommand("LOG_PLAYER_MOVE_FASTER", [&](std::vector<std::string> &args) -> std::string
    {
        getStatusManager().addStatusResetDuration(StatusManager::MOVEMENTSPEED_UP, 1);

        return "Player is red so player goes fastah -Random Ork Warhammer 40K (you go faster)";
    });

    win->registerCommand("LOG_PLAYER_MOVE_SLOWER", [&](std::vector<std::string> &args) -> std::string
    {
        getStatusManager().addStatusResetDuration(StatusManager::MOVEMENTSPEED_DOWN, 1);

        return "Player is not red so player does not go fastah -Random Ork Warhammer 40K (you go slower)";
    });
    win->registerCommand("LOG_PLAYER_HEAL", [&](std::vector<std::string> &args) -> std::string
    {
        getStatusManager().addStatusResetDuration(StatusManager::HEALTH_P1, 1);

        return "I need healing - Genji Shinimada ( + 1 Health given)";
    });
    win->registerCommand("LOG_PRINT_POS", [&](std::vector<std::string> &para) -> std::string 
    {
        return "x: " + std::to_string((double) getPosition().x) + ", y: " + std::to_string((double) getPosition().y) + ", z: " + std::to_string((double) getPosition().z);
    });
    win->registerCommand("LOG_PRINT_PHYSICS_VELOCITY", [&](std::vector<std::string> &para) -> std::string
    {
        return "x: " + std::to_string((double)m_charController->getLinearVelocity().x()) + ", y: " + std::to_string((double)m_charController->getLinearVelocity().y()) + ", z: " + std::to_string((double)m_charController->getLinearVelocity().z());
    });
    win->registerCommand("LOG_INCREASE_DAMAGE", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::P1_DAMAGE);

        return "20% extra damage";
    });
    win->registerCommand("LOG_INCREASE_MOVEMENT_SPEED", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::P20_PERC_MOVEMENTSPEED);

        return "Player is permanently 20% faster";
    });
    win->registerCommand("LOG_DECREASE_SKILL_CD", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::M20_PERC_CD);

        return "Player skills take 20% less time to recover";
    });
    win->registerCommand("LOG_INCREASE_MAG_SIZE", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::P40_MAGSIZE);

        return "Mag clip holds 40 more bullets";
    });
    win->registerCommand("LOG_INCREASE_AMMO_CAP", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::P20_AMMOCAP);

        return "You have bigger ammo bags now, it can hold 20 ammo more";
    });
    win->registerCommand("LOG_INCREASE_FIRE_RATE", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::P20_PERC_RATE_OF_FIRE);

        return "You shoot 20% faster";
    });
    win->registerCommand("LOG_INCREASE_RELOAD_TIME", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::M33_PERC_RELOAD_SPEED);

        return "You reload 20% faster";
    });
    win->registerCommand("LOG_INCREASE_JUMP_HEIGHT", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::P45_PERC_JUMP);

        return "You jump 45% higher";
    });
    win->registerCommand("LOG_PLAYER_SET_BURNING", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::FIRE_UPGRADE);

        return "Crossbow deals fire damage";
    });
    win->registerCommand("LOG_PLAYER_SET_FROST", [&](std::vector<std::string> &args)->std::string
    {
        upgrade(StatusManager::FROST_UPGRADE);

        return "Frost Staff has longer freezing effect";
    });
    win->registerCommand("LOG_GIVE_ENHANCE_AMMO", [&](std::vector<std::string> &para)->std::string
    {
        try {
            m_weaponManager->getWeaponLoadout(stoi(para[0]))->ammoContainer.setEnhancedAmmo(stoi(para[1]));
            return "Added ammo";
        }
        catch (std::exception &ex)
        {
            return "That didn't work";
        }
    });
}

void Player::clear()
{
	m_weaponManager->clear();
    delete m_weaponManager;
    delete m_skillManager;
	delete m_charController;
    delete m_listenerData;
}

void Player::reset()
{
    m_charController->setLinearVelocity({ 0.f, 0.f, 0.f });
    m_moveDir = { 0.f, 0.f, 0.f };
    m_moveSpeed = 0.f;
	m_charController->warp(startPosition);
    m_lastPos = getPositionBT();
	m_weaponManager->reset();
    m_skillManager->reset();
    currentWeapon = 0;
	m_hp = 3;
    currentSkills[0] = -1;;
    currentSkills[1] = -1;
    m_moveSpeedMod = 1.0f;
    m_permanentSpeedMod = 1.0f;
    m_jumpSpeedMod = 1.0f;
    m_stunned = false;
    m_damageTintTimer = 0.f;
    m_upgradeTintTimer = 0.f;
    m_pickupTintTimer = 0.f;

    getStatusManager().clear();
    getStatusManager().addUpgrade(StatusManager::FIRE_UPGRADE);

    //temp? probably
    Global::mainCamera->update(getPosition(), m_forward, Global::context);
    static SpecialEffectRenderInfo info = {};
    info.type = info.Snow;
    info.restart = true;

    QueueRender(info);
}

void Player::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    if (Projectile* p = dynamic_cast<Projectile*>(&other))	onCollision(*p);										// collision with projectile
    else if (Trigger* t = dynamic_cast<Trigger*>(&other)) {}														// collision with trigger
    else if (Enemy *e = dynamic_cast<Enemy*> (&other)) {}
}

void Player::onCollision(Projectile& other)
{
	if (other.getProjectileData().enemyBullet)
		takeDamage((int)other.getProjectileData().damage);
}

void Player::affect(int stacks, Effect const &effect, float deltaTime)
{
	long long flags = effect.getStandards()->flags;
    
    if (flags & Effect::EFFECT_BOUNCE)
    {
        m_charController->jump({ 0.f, PLAYER_JUMP_SPEED * 3, 0.f });
        m_playerState = PlayerState::IN_AIR;
    }
    if (flags & Effect::EFFECT_MODIFY_HP)
    {
        m_hp += static_cast<int> (effect.getModifiers()->modifyHP);
        if (m_hp > 3) m_hp = 3;
    }
    if (flags & Effect::EFFECT_IS_STUNNED)
    {
        //m_moveSpeedMod *= effect.getModifiers()->modifyMovementSpeed;
        m_stunned = true;
    }
    /*if (flags & Effect::EFFECT_MOVE_FASTER)
    {
        m_moveSpeedMod *= std::pow(effect.getModifiers()->modifyMovementSpeed, stacks);
        m_moveMaxSpeed *= m_moveSpeedMod;
    }
    if (flags & Effect::EFFECT_MOVE_SLOWER)
    {
        m_moveSpeedMod *= std::pow(effect.getModifiers()->modifyMovementSpeed, stacks);
        m_moveMaxSpeed *= m_moveSpeedMod;
    }
    if (flags & Effect::EFFECT_IS_FROZEN)
    {
        m_moveSpeedMod *= std::pow(effect.getSpecifics()->isFreezing, stacks);
        m_moveMaxSpeed *= m_moveSpeedMod;
    }*/
    if (flags & Effect::EFFECT_IS_WEAPON)
    {
        m_weaponManager->affect(effect);
    }
}

void Player::onEffectEnd(int stacks, Effect const & effect)
{
    long long flags = effect.getStandards()->flags;

    if (flags & Effect::EFFECT_IS_FROZEN)
    {
        /*m_moveSpeedMod = 1.0f;
        m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED * m_permanentSpeedMod;*/
    }
    if (flags & Effect::EFFECT_IS_STUNNED)
    {
        /*m_moveSpeedMod = 1.0f;
        m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED * m_permanentSpeedMod;*/
        m_stunned = false;
    }

    if (flags & Effect::EFFECT_MOVE_FASTER)
    {
       /* m_moveSpeedMod = 1.0f;
        m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED * m_permanentSpeedMod;*/
    }
    if (flags & Effect::EFFECT_MOVE_SLOWER)
    {
       /* m_moveSpeedMod = 1.0f;
        m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED * m_permanentSpeedMod;*/
    }
}

void Player::onUpgradeAdd(int stacks, Upgrade const & upgrade)
{
    long long flags = upgrade.getTranferEffects();

    m_upgradeTintTimer += 1.f;

    if (flags & Upgrade::UPGRADE_INCREASE_JUMPHEIGHT)
    {
        m_jumpSpeedMod += upgrade.getFlatUpgrades().movementSpeed;
    }
    if (flags & Upgrade::UPGRADE_IS_WEAPON)
    {
        m_weaponManager->onUpgradeAdd(stacks, upgrade);
    }
    if (flags & Upgrade::UPGRADE_IS_SKILL)
    {
        for (int i = 0; i < 2; i++)
        {
            getSkillManager()->getSkill(i)->upgradeAdd(stacks, upgrade);
        }
    }
    if (flags & Upgrade::UPGRADE_INCREASE_MOVEMENTSPEED)
    {
        m_permanentSpeedMod += upgrade.getFlatUpgrades().movementSpeed;
    }
}

void Player::updateSound(float deltaTime)
{
	// Update sound position
	btVector3 pos = getPositionBT();
	btVector3 vel = m_charController->getLinearVelocity();
	getSoundSource()->pos = { pos.x(), pos.y(), pos.z() };
    getSoundSource()->vel = { vel.x(), vel.y(), vel.z() };
    getSoundSource()->update(deltaTime);
}

void Player::saveToFile()
{

}

void Player::readFromFile()
{

}

void Player::takeDamage(int damage, bool damageThroughProtection)
{
    if (damage == 0) return;
    if (!m_godMode)
    {
        if (damageThroughProtection ||
            getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_INVINC) == 0)
        {
            getSoundSource()->playSFX(Sound::SFX::NECROMANCER_DEATH);
            m_hp -= damage;

            // Add invul time
            getStatusManager().addStatus(StatusManager::EFFECT_ID::INVULNERABLE, 1);
            
            SpecialEffectRenderInfo shake = {};
            shake.duration = 0.5f;
            shake.radius = 30.0f;
            shake.type = SpecialEffectRenderInfo::screenShake;
            shake.affectEveryThing = true;
            QueueRender(shake);

            m_damageTintTimer += 1.f;
        }
    }
}

int Player::getHP() const
{
	return m_hp;
}

int Player::getMaxHP() const
{
    return MAX_HP;
}

void Player::updateSpecific(float deltaTime)
{
	Player::update(deltaTime);

    // Updating screen tint
    updateScreenTint(deltaTime);

    // Update weapon
    m_weaponManager->update(deltaTime);

    // Updates listener info for sounds
    btVector3 up = { 0, 1, 0 };
    btVector3 forward = getForwardBT();
    btVector3 right = up.cross(forward);
    btVector3 actualUp = right.cross(forward);
    m_listenerData->update({ 0, 0, 0 }, actualUp.normalize(), { m_forward.x, m_forward.y, m_forward.z }, m_charController->getGhostObject()->getWorldTransform().getOrigin());

    // Get Mouse and Keyboard states for this frame
    DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
    DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();

    // Temp for testing
#ifdef _DEBUG
    if (ks.IsKeyDown(DirectX::Keyboard::B))
    {
        m_charController->warp({ 0.f, 0.f, 0.f });
        m_charController->setLinearVelocity({ 0.f, 0.f, 0.f });
        m_moveDir = { 0.f, 0.f, 0.f };
        m_moveSpeed = 0.f;
    }

    // TEMP FREE MOVE
    if (ks.IsKeyDown(DirectX::Keyboard::N) && !m_noclip)
    {
        m_charController->setGravity({ 0.f, 0.f, 0.f }); // remove gravity
        m_noclip = true;
        printf("free move activated\n");
    }
    else if (ks.IsKeyDown(DirectX::Keyboard::M) && m_noclip)
    {
        m_charController->setGravity({ 0.f, -PLAYER_GRAVITY, 0.f });
        // reset movement
        m_moveDir.setZero();
        m_moveSpeed = 0.f;
        m_noclip = false;
        printf("free move deactivated\n");
    }
#endif // !_DEBUG

    //Only allowed if not stunned
    if (!m_stunned)
    {
        // Movement
        if (ms.positionMode == DirectX::Mouse::MODE_RELATIVE)
            mouseMovement(deltaTime, &ms);
        jump(deltaTime, &ks);

	    // Get movement input
	    moveInput(&ks);
	    if (!m_noclip)
	    {
		    if (m_playerState == PlayerState::STANDING)
			    // Move
			    move(deltaTime);
		    else if (m_playerState == PlayerState::IN_AIR)
			    // Move in air
			    airMove(deltaTime);
	    }
	    else
		    moveFree(deltaTime, &ks);

	    if (m_charController->onGround())
	    {
            if (m_wasInAir)
            {
                SpecialEffectRenderInfo shake = {};
                shake.type = SpecialEffectRenderInfo::screenShake;
                shake.duration = 0.14f;
                shake.radius = 7.0f;
                shake.affectEveryThing = false;
                shake.direction = DirectX::SimpleMath::Vector2(0.5f, 1.0f);
                QueueRender(shake);
                m_wasInAir = false;
            }
		    m_playerState = PlayerState::STANDING;
		    m_charController->setLinearVelocity({ 0.f, 0.f, 0.f });
	    }
        else 
        {
            m_playerState = PlayerState::IN_AIR;
            m_wasInAir = true;
        }
		    

	    // Print player velocity
	    //printf("velocity: %f\n", m_moveSpeed);
	    //printf("%f\n", m_charController->getLinearVelocity().y());
	    //printf("%f	x: %f	z: %f\n", m_moveSpeed, m_moveDir.x(), m_moveDir.z());

	    //crouch(deltaTime);
        
        static int lastMouseScrollState = 0;
	    // Weapon swap
        if (ks.IsKeyDown(m_switchWeaponOne))
        {
            getSoundSource()->playSFX(Sound::SFX::SWOOSH);
            m_weaponManager->switchWeapon(0);
            currentWeapon = 0;
        }
		else if (ks.IsKeyDown(m_switchWeaponTwo))
        {
            getSoundSource()->playSFX(Sound::SFX::SWOOSH);
            m_weaponManager->switchWeapon(1);
            currentWeapon = 1;
        }
		else if (ks.IsKeyDown(m_switchWeaponThree))
        {
            getSoundSource()->playSFX(Sound::SFX::SWOOSH);
            m_weaponManager->switchWeapon(2);
            currentWeapon = 2;
        }
        else if (ms.scrollWheelValue > lastMouseScrollState)
        {
            getSoundSource()->playSFX(Sound::SFX::SWOOSH);
            currentWeapon--;
            currentWeapon = currentWeapon < 0 ? 2 : currentWeapon;
            m_weaponManager->switchWeapon(currentWeapon);
        }
        else if (ms.scrollWheelValue < lastMouseScrollState)
        {
            getSoundSource()->playSFX(Sound::SFX::SWOOSH);
            currentWeapon++;
            currentWeapon %= 3;
            m_weaponManager->switchWeapon(currentWeapon);
        }
        lastMouseScrollState = ms.scrollWheelValue;
		
	    // Skills
        PROFILE_BEGIN("SkillManager");
        forward = getForwardBT();
        if (ks.IsKeyDown(m_useSkillPrimary))
            m_skillManager->use(SkillManager::ID::PRIMARY, forward, *this);
        if (ks.IsKeyUp(m_useSkillPrimary))
            m_skillManager->release(SkillManager::ID::PRIMARY);
        if (ks.IsKeyDown(m_useSkillSecondary))
            m_skillManager->use(SkillManager::ID::SECONDARY, forward, *this);
        if (ks.IsKeyUp(m_useSkillSecondary))
            m_skillManager->release(SkillManager::ID::SECONDARY);
        if (ks.IsKeyDown(m_useSkillTertiary))
            m_skillManager->use(SkillManager::ID::TERTIARY, forward, *this);
        if (ks.IsKeyUp(m_useSkillTertiary))
            m_skillManager->release(SkillManager::ID::TERTIARY);
        PROFILE_END();

        // Check if reloading
        if (!m_weaponManager->isReloading() && ms.positionMode == DirectX::Mouse::MODE_RELATIVE)
        {
            // Primary and secondary attack
            if ((ms.leftButton))
                m_weaponManager->tryAttack(WEAPON_PRIMARY, getPositionBT() + btVector3(PLAYER_EYE_OFFSET) + getForwardBT(), m_camYaw, m_camPitch, *this);
            else if (ms.rightButton)
                m_weaponManager->tryAttack(WEAPON_SECONDARY, getPositionBT() + btVector3(PLAYER_EYE_OFFSET) + getForwardBT(), m_camYaw, m_camPitch, *this);

            // Reload
            if (ks.IsKeyDown(m_reloadWeapon))
                m_weaponManager->reloadWeapon();
        }
    }

    // update SkillManager
    m_skillManager->update(deltaTime);

    // saving position from last frame
    m_lastPos = getPositionBT();
    // step player
    if (!m_noclip)
        stepPlayer(deltaTime);

    if (!(DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::J)))
    {
        Global::mainCamera->update(getEyePosition(), m_forward, Global::context);
    }


    m_weaponManager->setWeaponModel(deltaTime, getEyeTransformMatrix(), m_forward);

    // for handling death
    if (getPositionBT().y() < MIN_Y)
        m_hp = 0;
}

void Player::moveInput(DirectX::Keyboard::State * ks)
{
	// Reset wish direction
	m_wishDir.setZero();
	m_wishDirForward = 0.f;
	m_wishDirRight = 0.f;

	// Move Left
	if (ks->IsKeyDown(m_moveLeft))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).cross({ 0, 1, 0 }).normalize();
		m_wishDir += -dir;
		m_wishDirRight += -1.f;
	}

	// Move Right
	if (ks->IsKeyDown(m_moveRight))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).cross({ 0, 1, 0 }).normalize();
		m_wishDir += dir;
		m_wishDirRight += 1.f;
	}

	// Move Forward
	if (ks->IsKeyDown(m_moveForward))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).normalize();
		m_wishDir += dir;
		m_wishDirForward += 1.f;
	}

	// Move Back
	if (ks->IsKeyDown(m_moveBack))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).normalize();
		m_wishDir += -dir;
		m_wishDirForward += -1.f;
	}

	// Normalize movement direction
	if (!m_wishDir.isZero())
		m_wishDir = m_wishDir.safeNormalize();
}

void Player::moveFree(float deltaTime, DirectX::Keyboard::State * ks)
{
	if (ks->IsKeyDown(DirectX::Keyboard::Keys::LeftControl)) // down
		m_wishDir.setY(-0.1f * deltaTime);
	if (ks->IsKeyDown(DirectX::Keyboard::Keys::Space))		 // up
		m_wishDir.setY(0.1f * deltaTime);

	// Update pos of player
    m_charController->warp(getPositionBT() + (m_wishDir * m_moveMaxSpeed * 2.f * deltaTime));
}

void Player::move(float deltaTime)
{
	// On ground
    if (!m_wishJump)
	{
        m_firstJump = true;
		float friction = (m_moveMaxSpeed * m_permanentSpeedMod * 2 - (m_moveMaxSpeed * m_permanentSpeedMod - m_moveSpeed)) * PLAYER_FRICTION; // smooth friction
		applyFriction(deltaTime, friction > PLAYER_FRICTION_MIN ? friction : PLAYER_FRICTION_MIN);

		// if player wants to move
		if (!m_wishDir.isZero())
		{
			// Reset movement speed if changing direction
			if (m_moveDir.dot(m_wishDir) <= 0.f)
				m_moveSpeed = 0.f;

			// Change move direction
			m_moveDir = m_wishDir;
		}
        else if (m_moveSpeed < FLT_EPSILON)
        {
            m_moveDir = { 0.f, 0.f, 0.f };
        }
	}
	// On ground and about to jump
	else
	{
        m_airAcceleration = (PLAYER_SPEED_LIMIT - m_moveSpeed) * PLAYER_MOVEMENT_AIRACCELERATION;

        if (!m_firstJump) // first jump no friction
        {
            if (!m_wishDir.isZero() && m_moveDir.dot(m_wishDir) <= 0.f)
                applyAirFriction(deltaTime, PLAYER_AIR_FRICTION * 6.f);		// if trying to move in opposite direction in air apply more friction
            else
                applyAirFriction(deltaTime, PLAYER_AIR_FRICTION);
        }
	}

	// Apply acceleration and move player
    if (m_wishJump && m_firstJump) // first jump accel
        accelerate(deltaTime, m_airAcceleration);
    else if (m_wishDir.isZero() || m_wishJump)
		accelerate(deltaTime, 0.f);
	else
		accelerate(deltaTime, m_acceleration);

	// Apply jump if player wants to jump
	if (m_wishJump)
	{
        m_firstJump = false;
        getSoundSource()->playSFX(Sound::SFX::JUMP, 1.f, 0.1f);
		m_charController->jump({ 0.f, PLAYER_JUMP_SPEED * m_jumpSpeedMod, 0.f });
		m_wishJump = false;
	}
}

void Player::airMove(float deltaTime)
{
	applyAirFriction(deltaTime, (m_moveMaxSpeed * m_permanentSpeedMod - (m_moveMaxSpeed * m_permanentSpeedMod - m_moveSpeed)) * PLAYER_AIR_FRICTION); // smooth friction

	accelerate(deltaTime, m_airAcceleration);

	m_airAcceleration = 0.f;
}

void Player::accelerate(float deltaTime, float acceleration)
{
    m_moveSpeed += acceleration * deltaTime * m_moveSpeedMod * m_permanentSpeedMod;
    if (m_playerState != PlayerState::IN_AIR && !m_wishJump && m_moveSpeed > (m_moveMaxSpeed * m_permanentSpeedMod))
        m_moveSpeed = m_moveMaxSpeed * m_permanentSpeedMod;
}

void Player::stepPlayer(float deltaTime)
{
    if (deltaTime * 0.001f > (1.f / 60.f))
        deltaTime = (1.f / 60.f) * 1000.f;

    // Apply moveDir and moveSpeed to player
    if (m_playerState != PlayerState::IN_AIR)
        m_charController->setVelocityForTimeInterval(m_moveDir * m_moveSpeed, deltaTime);
    else
        m_charController->setVelocityForTimeInterval(((m_moveDir + btVector3(0.f, m_charController->getLinearVelocity().y(), 0.f)) * m_moveSpeed) + (m_wishDir * PLAYER_MOVEMENT_AIRSTRAFE_SPEED), deltaTime);

    PROFILE_BEGIN("Stepping player");
    // Step player
    m_charController->preStep(m_physPtr);
    m_charController->playerStep(m_physPtr, deltaTime);
    PROFILE_END();

    // if new position is totally fucked for some reason, reset to last position
    if (abs(m_lastPos.x() - getPositionBT().x()) > PLAYER_MAX_MOVE ||
        abs(m_lastPos.y() - getPositionBT().y()) > PLAYER_MAX_MOVE ||
        abs(m_lastPos.z() - getPositionBT().z()) > PLAYER_MAX_MOVE)
        m_charController->warp(m_lastPos);
}

void Player::applyFriction(float deltaTime, float friction)
{
	float toDrop = m_deacceleration * deltaTime * friction;

	m_moveSpeed -= toDrop;
	if (m_moveSpeed < 0)
		m_moveSpeed = 0;
}

void Player::applyAirFriction(float deltaTime, float friction)
{
	btVector3 rightMoveDir = m_moveDir.cross(btVector3(0.f, 1.f, 0.f));

	btVector3 forward = btVector3(m_forward.x, 0.f, m_forward.z).safeNormalize(); // forward vector (look direction)

	float lookMoveAngle = m_moveDir.dot(forward);

	float lookMovedirection = rightMoveDir.dot(forward);

	// if looking backwards compared to move direction
	if (lookMoveAngle < 0.f)
	{
		lookMoveAngle *= -1.f;
		forward *= -1.f;
	}

	if (lookMovedirection < 0.f && m_wishDirRight < 0.f)
	{
		if (lookMoveAngle > PLAYER_STRAFE_ANGLE)
			m_moveDir = (m_moveDir + forward) * 0.5f;
		else
		{
			m_airAcceleration = 0.f;
			applyFriction(deltaTime, friction);
		}
	}
	else if (lookMovedirection > 0.f && m_wishDirRight > 0.f)
	{
		if (lookMoveAngle > PLAYER_STRAFE_ANGLE)
			m_moveDir = (m_moveDir + forward) * 0.5f;
		else
		{
			m_airAcceleration = 0.f;
			applyFriction(deltaTime, friction);
		}
	}
	else
	{
		m_airAcceleration = 0.f;
		applyFriction(deltaTime, friction);
	}
}


void Player::jump(float deltaTime, DirectX::Keyboard::State* ks)
{
    if (ks->IsKeyDown(m_jump) && !m_wishJump && m_playerState != PlayerState::IN_AIR) {
        SpecialEffectRenderInfo bounceInfo;
        bounceInfo.type = bounceInfo.screenBounce;
        bounceInfo.duration = 0.17f;
        bounceInfo.radius = 140.0f;
        bounceInfo.bounceMax = 5.0f;
        bounceInfo.direction = DirectX::SimpleMath::Vector2(0.0f, 1.0f);
        bounceInfo.affectEveryThing = false;
        QueueRender(bounceInfo);
        m_wishJump = true;
    }	
	else if (ks->IsKeyUp(m_jump))
		m_wishJump = false;
}

void Player::crouch(float deltaTime)
{
	// crouch
}

void Player::mouseMovement(float deltaTime, DirectX::Mouse::State * ms)
{
    if (ms->positionMode == DirectX::Mouse::MODE_RELATIVE)
    {
        Settings& setting = Settings::getInstance();

        // so bad lol
        POINT mousePos;
        GetCursorPos(&mousePos);
        auto vec2 = getWindowMidPoint();

        int xDiff = mousePos.x - vec2.x;
        int yDiff = mousePos.y - vec2.y;

        m_camYaw += setting.getMouseSense() * xDiff;
        m_camPitch -= setting.getMouseSense() * yDiff;

        SetCursorPos(vec2.x, vec2.y);
    }

	// DirectX calculates position on the full resolution,
	//  while getWindowMidPoint gets the current window's middle point!!!!!

	 // Pitch lock and yaw correction
	if (m_camPitch > 89)
		m_camPitch = 89;
	if (m_camPitch < -89)
		m_camPitch = -89;
	if (m_camYaw < 0.f)
		m_camYaw += 360.f;
	if (m_camYaw > 360.f)
		m_camYaw -= 360.f;

	// Reset cursor to mid point of window
//	SetCursorPos(midPoint.x, midPoint.y);

	// Create forward
	m_forward.x = cos(DirectX::XMConvertToRadians(m_camPitch)) * cos(DirectX::XMConvertToRadians(m_camYaw));
	m_forward.y = sin(DirectX::XMConvertToRadians(m_camPitch));
	m_forward.z = cos(DirectX::XMConvertToRadians(m_camPitch)) * sin(DirectX::XMConvertToRadians(m_camYaw));

	m_forward.Normalize();
}

DirectX::SimpleMath::Vector2 Player::getWindowMidPoint()
{
    RECT rect;
    GetWindowRect(*Engine::g_window, &rect);

    return DirectX::SimpleMath::Vector2(std::floor((rect.left + rect.right * 0.5f)), std::floor((rect.top + rect.bottom * 0.5f))); // Returns mid point for window
}

void Player::updateScreenTint(float deltaTime)
{
    m_damageTintTimer -= deltaTime / 1000.f;
    if (m_damageTintTimer < 0.f) m_damageTintTimer = 0.f;

    m_upgradeTintTimer -= deltaTime / 1000.f;
    if (m_upgradeTintTimer < 0.f) m_upgradeTintTimer = 0.f;

    m_pickupTintTimer -= deltaTime / 1000.f;
    if (m_pickupTintTimer < 0.f) m_pickupTintTimer = 0.f;

    SpecialEffectRenderInfo tint = {};
    if (m_damageTintTimer > 0.f)
    {
        tint.type = SpecialEffectRenderInfo::Tint;
        tint.color = DirectX::SimpleMath::Vector3(1, 0, 0);
        tint.progress = m_damageTintTimer;
        QueueRender(tint);
    }
    else if(m_upgradeTintTimer > 0.f)
    {
        tint.type = SpecialEffectRenderInfo::Tint;
        tint.color = DirectX::SimpleMath::Vector3(1, 1, 1);
        tint.progress = m_upgradeTintTimer;
        QueueRender(tint);
    }
    else if (m_pickupTintTimer > 0.f)
    {
        tint.type = SpecialEffectRenderInfo::Tint;
        tint.color = DirectX::SimpleMath::Vector3(1, 1, 1);
        tint.progress = m_pickupTintTimer;
        QueueRender(tint);
    }
}

btKinematicCharacterController * Player::getCharController()
{
	return m_charController;
}

btGhostObject* Player::getGhostObject()
{
	return m_charController->getGhostObject();
}

DirectX::SimpleMath::Matrix Player::getTransformMatrix() const
{
	// Making memory for a matrix
	float* m = newd float[4 * 16];

	// Getting this entity's matrix
	m_charController->getGhostObject()->getWorldTransform().getOpenGLMatrix((btScalar*)(m));

	// Translating to DirectX Math and assigning the variables
	DirectX::SimpleMath::Matrix transformMatrix(m);

	//Find the scaling matrix
	auto scale = DirectX::SimpleMath::Matrix::CreateScale(PLAYER_SIZE_RADIUS * 2, PLAYER_SIZE_HEIGHT * 2, PLAYER_SIZE_RADIUS * 2);

	// Deleting the old created variables from memory
	delete m;

	return scale * transformMatrix;
}

DirectX::SimpleMath::Matrix Player::getEyeTransformMatrix() const
{
    // Making memory for a matrix
    float* m = newd float[4 * 16];

    // Getting this entity's matrix
    btTransform tr = m_charController->getGhostObject()->getWorldTransform();
    tr.setOrigin(getPositionBT() + btVector3(PLAYER_EYE_OFFSET));
    tr.getOpenGLMatrix((btScalar*)(m));

    // Translating to DirectX Math and assigning the variables
    DirectX::SimpleMath::Matrix transformMatrix(m);

    //Find the scaling matrix
    auto scale = DirectX::SimpleMath::Matrix::CreateScale(PLAYER_SIZE_RADIUS * 2, PLAYER_SIZE_HEIGHT * 2, PLAYER_SIZE_RADIUS * 2);

    // Deleting the old created variables from memory
    delete m;

    return scale * transformMatrix;
}

void Player::render() const
{
	// Drawing the actual player model (can be deleted later, cuz we don't need it, unless we expand to multiplayer)
	//Object::render(renderer);

	// Drawing the weapon model
	m_weaponManager->render();
	m_skillManager->render();
}

void Player::setMaxSpeed(float maxSpeed)
{
	m_moveMaxSpeed = maxSpeed;
}

DirectX::SimpleMath::Vector3 Player::getPosition() const
{
	return DirectX::SimpleMath::Vector3(m_charController->getGhostObject()->getWorldTransform().getOrigin());
}

DirectX::SimpleMath::Vector3 Player::getEyePosition() const
{
    return DirectX::SimpleMath::Vector3(m_charController->getGhostObject()->getWorldTransform().getOrigin() + btVector3(PLAYER_EYE_OFFSET));
}

btVector3 Player::getPositionBT() const
{
	return m_charController->getGhostObject()->getWorldTransform().getOrigin();
}

btTransform& Player::getTransform() const
{
	return m_charController->getGhostObject()->getWorldTransform();
}

float Player::getYaw() const
{
    return m_camYaw;
}

float Player::getPitch() const
{
    return m_camPitch;
}

float Player::getMoveSpeed() const
{
	return m_moveSpeed;
}

void Player::setMoveSpeed(float speed)
{
	m_moveSpeed = speed;
}

void Player::setMoveDirection(btVector3 moveDir)
{
	m_moveDir = moveDir;
}

btVector3 Player::getForwardBT()
{
	return btVector3(m_forward.x, m_forward.y, m_forward.z);
}

DirectX::SimpleMath::Vector3 Player::getForward() const
{
	return m_forward;
}

btVector3 Player::getMoveDirection()
{
	return m_moveDir;
}

void Player::setPlayerState(PlayerState playerState)
{
	m_playerState = playerState;
}

Player::PlayerState Player::getPlayerState() const
{
	return m_playerState;
}

Sound::ListenerData& Player::getListenerData()
{
	return *m_listenerData;
}

SkillManager* Player::getSkillManager()
{
    return m_skillManager;
}
const AmmoContainer& Player::getActiveAmmoContainer() const
{
    return m_weaponManager->getActiveWeaponLoadout()->ammoContainer;
}

const AmmoContainer& Player::getInactiveAmmoContainer() const
{
    return m_weaponManager->getInactiveWeaponLoadout()->ammoContainer;
}

const Skill* Player::getSkill(int id) const
{
    return m_skillManager->getSkill(id);
}

bool Player::isUsingMeleeWeapon() const
{
    return m_weaponManager->getCurrentWeaponLoadout()->ammoContainer.getAmmoInfo().ammoConsumption[WEAPON_PRIMARY] == 0;
}

int Player::getCurrentWeapon() const
{
    return currentWeapon;
}

void Player::setCurrentSkills(int first, int second)
{
    m_skillManager->switchToSkill({ SkillManager::SKILL(second), SkillManager::SKILL(first) });
    currentSkills[0] = first;
    currentSkills[1] = second;
}

int Player::getCurrentSkill1() const
{
    return currentSkills[1];
}

bool Logic::Player::getReloding() const
{
    return m_weaponManager->isReloading();;
}

int Logic::Player::getAmmoPickedUp()
{
    return m_weaponManager->getAmmoPickedUp();;
}

void Logic::Player::setPickupTintTimer(float time)
{
    m_pickupTintTimer = time;
}

void Player::setTargetedBy(Entity *entity)
{
    m_targetedBy = entity;
}

bool Player::isTargeted()
{
    return m_targetedBy;
}

void Player::resetTargeted()
{
    m_targetedBy = nullptr;
}

int Player::getCurrentSkill0() const
{
    return currentSkills[0];
}