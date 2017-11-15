#include "Player/Player.h"

#include <DebugDefines.h>
#include <Misc\ComboMachine.h>

#include <AI\EnemyTest.h>
#include <AI\Trigger.h>

#include <Projectile\ProjectileManager.h>

#include <Player\Weapon\WeaponManager.h>
#include <Player\Weapon\Weapon.h>
#include <Player\Weapon\AmmoContainer.h>

#include <Player\Skill\SkillManager.h>
#include <Player\Skill\Skill.h>

#include <Misc\Sound\NoiseStructs.h>
#include <Physics\Physics.h>
#include <Projectile\Projectile.h>

#include <Engine\Profiler.h>
#include <Engine\DebugWindow.h>
#include <Graphics\include\MainCamera.h>
#include <Graphics\include\Device.h>

#include <Engine/Settings.h>

using namespace Logic;

btVector3 Player::startPosition = btVector3(0.f, 6.f, 0.f);

Player::Player(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent)
: Entity(body, halfExtent)
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
    Settings setting = Settings::getInstance();
	m_weaponManager->init(projectileManager);
	m_skillManager->init(physics, projectileManager);
	m_physPtr = physics;

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

	m_forward = DirectX::SimpleMath::Vector3(0, 0, 1);
	m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED;
	m_moveDir.setZero();
	m_moveSpeed = 0.f;
    m_moveSpeedMod = 1.0f;
	m_acceleration = PLAYER_MOVEMENT_ACCELERATION;
	m_deacceleration = m_acceleration * 0.5f;
	m_airAcceleration = PLAYER_MOVEMENT_AIRACCELERATION;
	m_jumpSpeed = PLAYER_JUMP_SPEED;
	m_wishDir.setZero();
	m_wishDirForward = 0.f;
	m_wishDirRight = 0.f;
	m_wishJump = false;

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
        getStatusManager().addStatus(StatusManager::STUN, 1, true);

        return "Player is stunned";
    });

    win->registerCommand("LOG_PLAYER_MOVE_FASTER", [&](std::vector<std::string> &args) -> std::string
    {
        getStatusManager().addStatus(StatusManager::MOVEMENTSPEED_UP, 1, true);

        return "Player is red so player goes fastah -Random Ork Warhammer 40K (you go faster)";
    });

    win->registerCommand("LOG_PLAYER_MOVE_SLOWER", [&](std::vector<std::string> &args) -> std::string
    {
        getStatusManager().addStatus(StatusManager::MOVEMENTSPEED_DOWN, 1, true);

        return "Player is not red so player does not go fastah -Random Ork Warhammer 40K (you go slower)";
    });

    win->registerCommand("LOG_PLAYER_ON_FIRE", [&](std::vector<std::string> &args) -> std::string
    {
        getStatusManager().addStatus(StatusManager::ON_FIRE, 1);

        return "You sir are on fire (You will take 1 damage soon)";
    });
    win->registerCommand("LOG_PLAYER_HEAL", [&](std::vector<std::string> &args) -> std::string
    {
        getStatusManager().addStatus(StatusManager::HEALTH_P1, 1);

        return "I need healing - Genji Shinimada ( + 1 Health given)";
    });
    win->registerCommand("LOG_PRINT_POS", [&](std::vector<std::string> &para) -> std::string 
    {
        return "x: " + std::to_string((double) getPosition().x) + ", y: " + std::to_string((double) getPosition().y) + ", z: " + std::to_string((double) getPosition().z);
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
	getTransform().setOrigin(startPosition);
	m_weaponManager->reset();
    m_skillManager->reset();
    currentWeapon = 0;
	m_hp = 3;

    //temp? probably
    Global::mainCamera->update(getPosition(), m_forward, Global::context);
    static SpecialEffectRenderInfo info;
    info.type = info.Snow;
    info.restart = true;

    QueueRender(info);
}

void Player::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    if (!m_godMode)
    {
        if (Projectile* p = dynamic_cast<Projectile*>(&other))	onCollision(*p);										// collision with projectile
        else if (Trigger* t = dynamic_cast<Trigger*>(&other)) {}														// collision with trigger
        else if (Enemy *e = dynamic_cast<Enemy*> (&other))
        {
            int stacks = getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_PUSH_BACK);
            e->getRigidBody()->applyCentralForce((getPositionBT() - e->getPositionBT()).normalize() * static_cast<btScalar> (stacks));
            stacks = getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_DAMAGE_ON_CONTACT);
            e->damage(2.f * stacks); // replace 1 with the player damage when it is better
        }
    }
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
    }
	if (flags & Effect::EFFECT_MODIFY_AMMO)
	{
        WeaponManager::WeaponLoadout* wp = nullptr;
        if(effect.getSpecifics()->ammoType == 0)
		    wp = m_weaponManager->getWeaponLoadout(0);
        else if(effect.getSpecifics()->ammoType == 1)
            wp = m_weaponManager->getWeaponLoadout(1);

        if (wp)
        {
            int magSize = wp->ammoContainer->getAmmoInfo().magSize;
            int currentAmmo = wp->ammoContainer->getAmmoInfo().ammo;
            if ((currentAmmo + magSize) > wp->ammoContainer->getAmmoInfo().ammoCap)
                wp->ammoContainer->setAmmo(wp->ammoContainer->getAmmoInfo().ammoCap);
            else
                wp->ammoContainer->setAmmo(currentAmmo + magSize);
        }
	}
    if (flags & Effect::EFFECT_IS_FROZEN)
    {
        m_moveSpeedMod = std::pow(effect.getSpecifics()->isFreezing, stacks);
        m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED * m_moveSpeedMod;
    }
    if (flags & Effect::EFFECT_IS_STUNNED)
    {
        m_moveSpeedMod = effect.getModifiers()->modifyMovementSpeed;
        m_stunned = true;
    }
    if (flags & Effect::EFFECT_MOVE_FASTER)
    {
        m_moveSpeedMod = std::pow(effect.getModifiers()->modifyMovementSpeed, stacks);
        m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED * m_moveSpeedMod;
    }
    if (flags & Effect::EFFECT_MOVE_SLOWER)
    {
        m_moveSpeedMod = std::pow(effect.getModifiers()->modifyMovementSpeed, stacks);
        m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED * m_moveSpeedMod;
    }
}

void Player::onEffectEnd(int stacks, Effect const & effect)
{
    long long flags = effect.getStandards()->flags;

    if (flags & Effect::EFFECT_ON_FIRE)
    {
        takeDamage(static_cast<int> (effect.getModifiers()->modifyDmgTaken));
    }
    if (flags & Effect::EFFECT_IS_FROZEN)
    {
        m_moveSpeedMod = 1;
    }
    if (flags & Effect::EFFECT_IS_STUNNED)
    {
        m_moveSpeedMod = 1;
        m_stunned = false;
    }

    if (flags & Effect::EFFECT_MOVE_FASTER)
    {
        m_moveSpeedMod = 1;
        m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED;
    }
    if (flags & Effect::EFFECT_MOVE_SLOWER)
    {
        m_moveSpeedMod = 1;
        m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED;
    }
}

void Player::upgrade(Upgrade const & upgrade)
{
	long long flags = upgrade.getTranferEffects();

	if (flags & Upgrade::UPGRADE_INCREASE_MAGSIZE)
	{

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
    if (!m_godMode)
    {
        if (damageThroughProtection ||
            getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_INVINC) == 0)
        {
            getSoundSource()->playSFX(Sound::SFX::NECROMANCER_DEATH);
            m_hp -= damage;

            // Add invul time
            getStatusManager().addStatus(StatusManager::EFFECT_ID::INVULNERABLE, 1);
        }
    }
}

int Player::getHP() const
{
	return m_hp;
}

void Player::updateSpecific(float deltaTime)
{
    Player::update(deltaTime);

    // Update weapon and skills
    m_weaponManager->update(deltaTime);
    m_skillManager->update(deltaTime);

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
		    m_playerState = PlayerState::STANDING;
		    m_charController->setLinearVelocity({ 0.f, 0.f, 0.f });
	    }
	    else
		    m_playerState = PlayerState::IN_AIR;

	    // Print player velocity
	    //printf("velocity: %f\n", m_moveSpeed);
	    //printf("%f\n", m_charController->getLinearVelocity().y());
	    //printf("%f	x: %f	z: %f\n", m_moveSpeed, m_moveDir.x(), m_moveDir.z());

	    //crouch(deltaTime);

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
                m_weaponManager->tryUsePrimary(getPositionBT() + getForwardBT(), m_camYaw, m_camPitch, *this);
            else if (ms.rightButton)
                m_weaponManager->tryUseSecondary(getPositionBT() + getForwardBT(), m_camYaw, m_camPitch, *this);

            // Reload
            if (ks.IsKeyDown(m_reloadWeapon))
                m_weaponManager->reloadWeapon();
        }
    }

    /* if (m_godMode)
    {*/
        static bool isNum = false;
        static bool wasNum = false;
        wasNum = isNum;
        isNum = ks.NumPad6;

        if (isNum && !wasNum)
            m_hp--;
    /*}*/

    Global::mainCamera->update(getEyePosition(), m_forward, Global::context);
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
		float friction = (m_moveMaxSpeed * 2 - (m_moveMaxSpeed - m_moveSpeed)) * PLAYER_FRICTION; // smooth friction
		applyFriction(deltaTime, friction > 0.1f ? friction : 0.1f);

		// if player wants to move
		if (!m_wishDir.isZero())
		{
			// Reset movement speed if changing direction
			if (m_moveDir.dot(m_wishDir) <= 0.f)
				m_moveSpeed = 0.f;

			// Change move direction
			m_moveDir = m_wishDir;
		}
	}
	// On ground and about to jump
	else
	{
		m_airAcceleration = (PLAYER_SPEED_LIMIT - m_moveSpeed) * PLAYER_MOVEMENT_AIRACCELERATION;
		
		if (!m_wishDir.isZero() && m_moveDir.dot(m_wishDir) <= 0.f)
			applyAirFriction(deltaTime, PLAYER_AIR_FRICTION * 6.f);		// if trying to move in opposite direction in air apply more friction
		else
			applyAirFriction(deltaTime, PLAYER_AIR_FRICTION);
	}

	// Apply acceleration and move player
	if(m_wishDir.isZero() || m_wishJump)
		accelerate(deltaTime, 0.f);
	else
		accelerate(deltaTime, m_acceleration);

	// Apply jump if player wants to jump
	if (m_wishJump)
	{
        getSoundSource()->playSFX(Sound::SFX::JUMP, 1.f, 0.1f);
		m_charController->jump({ 0.f, PLAYER_JUMP_SPEED, 0.f });
		m_wishJump = false;
	}
}

void Player::airMove(float deltaTime)
{
	applyAirFriction(deltaTime, (m_moveMaxSpeed - (m_moveMaxSpeed - m_moveSpeed)) * PLAYER_AIR_FRICTION); // smooth friction

	accelerate(deltaTime, m_airAcceleration);

	m_airAcceleration = 0.f;
}

void Player::accelerate(float deltaTime, float acceleration)
{
	if (deltaTime * 0.001f > (1.f / 60.f))
		deltaTime = (1.f / 60.f) * 1000.f;

	m_moveSpeed += acceleration * deltaTime * m_moveSpeedMod;

	if (m_playerState != PlayerState::IN_AIR && !m_wishJump && m_moveSpeed > m_moveMaxSpeed)
		m_moveSpeed = m_moveMaxSpeed;

	// Apply moveDir and moveSpeed to player
	if(m_playerState != PlayerState::IN_AIR)
		m_charController->setVelocityForTimeInterval(m_moveDir * m_moveSpeed, deltaTime);
	else
		m_charController->setVelocityForTimeInterval(((m_moveDir + btVector3(0.f, m_charController->getLinearVelocity().y(), 0.f)) * m_moveSpeed) + (m_wishDir * PLAYER_MOVEMENT_AIRSTRAFE_SPEED), deltaTime);

	PROFILE_BEGIN("Stepping player");
	// Step player
	m_charController->preStep(m_physPtr);
	m_charController->playerStep(m_physPtr, deltaTime);
	PROFILE_END()
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
	if (ks->IsKeyDown(m_jump) && !m_wishJump && m_playerState != PlayerState::IN_AIR)
		m_wishJump = true;
	else if (ks->IsKeyUp(m_jump))
		m_wishJump = false;
}

void Player::crouch(float deltaTime)
{
	// crouch
}

void Player::mouseMovement(float deltaTime, DirectX::Mouse::State * ms)
{
    Settings setting = Settings::getInstance();
	m_camYaw	+= setting.getMouseSense() * (ms->x * deltaTime);
	m_camPitch	-= setting.getMouseSense() * (ms->y * deltaTime);

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

	// Setting position of updated weapon and skill models
	m_weaponManager->setWeaponModel(getEyeTransformMatrix(), m_forward);
	//	m_skillManager->setWeaponModel(getTransformMatrix(), m_forward);

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
    return *m_weaponManager->getActiveWeaponLoadout()->ammoContainer;
}

const AmmoContainer& Player::getInactiveAmmoContainer() const
{
    return *m_weaponManager->getInactiveWeaponLoadout()->ammoContainer;
}

const Skill* Player::getSkill(int id) const
{
    return m_skillManager->getSkill(id);
}

bool Player::isUsingMeleeWeapon() const
{
    return m_weaponManager->getCurrentWeaponLoadout()->ammoContainer->getAmmoInfo().primAmmoConsumption == 0;
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