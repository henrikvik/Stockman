#include "Player/Player.h"
#include <AI\EnemyTest.h>
#include <AI\Trigger.h>
#include <DebugDefines.h>
#include <Misc\ComboMachine.h>
#include <Engine\Profiler.h>

using namespace Logic;

btVector3 Player::startPosition = btVector3(0.f, 6.f, 0.f);

Player::Player(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent)
: Entity(body, halfExtent, modelID)
{

}

Player::~Player()
{
	clear();
}

void Player::init(Physics* physics, ProjectileManager* projectileManager, GameTime* gameTime)
{
	m_weaponManager.init(projectileManager);
	m_skillManager.init(physics, projectileManager, gameTime);
	m_physPtr = physics;

	btCapsuleShape* playerShape = new btCapsuleShape(PLAYER_SIZE_HEIGHT, PLAYER_SIZE_RADIUS);
	btPairCachingGhostObject* ghostObject = m_physPtr->createPlayer(playerShape, startPosition);
	ghostObject->setUserPointer(this);

	m_charController = new btKinematicCharacterController(ghostObject, playerShape, 0.2f, btVector3(0.f, 1.f, 0.f));
	m_charController->setGravity({ 0.f, -PLAYER_GRAVITY, 0.f });
	m_physPtr->addAction(m_charController);

	// Stats
	m_hp = PLAYER_STARTING_HP;
    	info.hp = m_hp;
   	info.cuttleryAmmo[0] = 0;
   	info.cuttleryAmmo[1] = 0;
    	info.iceAmmo[0] = 0;
    	info.iceAmmo[1] = 0;
    	info.wave = 0;
   	info.score = 0;
   	info.sledge = false;
	info.cd = 1.0f;

	// Default mouse sensetivity, lookAt
	m_camYaw = 90;
	m_camPitch = 5;

	m_playerState = PlayerState::STANDING;
	m_mouseSens = PLAYER_MOUSE_SENSETIVITY;
	m_forward = DirectX::SimpleMath::Vector3(0, 0, 1);
	m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED;
	m_moveDir = btVector3(0, 0, 0);
	m_moveSpeed = 0.f;
	m_acceleration = PLAYER_MOVEMENT_ACCELERATION;
	m_deacceleration = m_acceleration * 0.5f;
	m_airAcceleration = PLAYER_MOVEMENT_AIRACCELERATION;
	m_jumpSpeed = PLAYER_JUMP_SPEED;
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
	m_useSkill = DirectX::Keyboard::Keys::E;
}

void Player::clear()
{
	m_weaponManager.clear();
	m_skillManager.clear();
	
	delete m_charController;
}

void Player::reset()
{
	getRigidBody()->getWorldTransform().setOrigin(startPosition);
	m_weaponManager.reset();
	m_hp = 3;
}

void Player::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
#ifdef GOD_MODE
	
#else
	if (Projectile* p	= dynamic_cast<Projectile*>(&other))	onCollision(*p);										// collision with projectile
	else if (Trigger* t = dynamic_cast<Trigger*>(&other))	 	{ }														// collision with trigger
	else if (Enemy *e = dynamic_cast<Enemy*> (&other))
	{
		int stacks = getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_PUSH_BACK);
		e->getRigidBody()->applyCentralForce((getPositionBT() - e->getPositionBT()).normalize() * stacks); 
		stacks = getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_DAMAGE_ON_CONTACT);
		e->damage(2 * stacks); // replace 1 with the player damage when it is better
	}
#endif
}

void Player::onCollision(Projectile& other)
{
	if (other.getProjectileData().enemyBullet)
		takeDamage(other.getProjectileData().damage);
}

void Player::affect(int stacks, Effect const &effect, float deltaTime)
{
	long long flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_MODIFY_MOVEMENTSPEED)
	{
		m_charController->jump({ 0.f, PLAYER_JUMP_SPEED * 3, 0.f });
		m_playerState = PlayerState::IN_AIR;
	}

	if (flags & Effect::EFFECT_MODIFY_AMMO)
	{
		Weapon* wp		= m_weaponManager.getCurrentWeaponPrimary();
		int magSize		= wp->getMagSize();
		int currentAmmo = wp->getAmmo();
		wp->setAmmo(currentAmmo + (magSize * WEAPON_AMMO_PACK_MODIFIER));
	}
}

void Player::upgrade(Upgrade const & upgrade)
{
	long long flags = upgrade.getTranferEffects();

	if (flags & Upgrade::UPGRADE_INCREASE_MAGSIZE)
	{

	}
}

void Player::saveToFile()
{

}

void Player::readFromFile()
{

}

void Player::takeDamage(int damage, bool damageThroughProtection)
{
#ifndef GOD_MODE
	if (damageThroughProtection ||
		getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_INVINC) == 0)
		m_hp -= damage;
#endif
}

int Player::getHP() const
{
	return m_hp;
}

void Player::updateSpecific(float deltaTime)
{
	Player::update(deltaTime);

	// Updates listener info for sounds
	btVector3 up		= { 0, 1, 0 };
	btVector3 forward	= getForwardBT();
	btVector3 right		= up.cross(forward);
	btVector3 actualUp	= right.cross(forward);
	m_listenerData.update({ 0, 0, 0 }, actualUp.normalize(), { m_forward.x, m_forward.y, m_forward.z }, m_charController->getGhostObject()->getWorldTransform().getOrigin());

    //updates hudInfo with the current info
	info.score = ComboMachine::Get().GetCurrentScore();
    info.hp = m_hp;
    info.cuttleryAmmo[0] = m_weaponManager.getfirstWeapon()->getMagAmmo();
    info.cuttleryAmmo[1] = m_weaponManager.getfirstWeapon()->getAmmo();
    info.iceAmmo[0] = m_weaponManager.getSecondWeapon()->getMagAmmo();
    info.iceAmmo[1] = m_weaponManager.getSecondWeapon()->getAmmo();
    if (m_weaponManager.getCurrentWeaponPrimary()->getMagSize() == 0)
    {
        info.sledge  = true;
    }
    else
    {
        info.sledge = false;
    }

    if (!m_skillManager.getCurrentSkill()->getCanUse())
    {
        info.cd = m_skillManager.getCurrentSkill()->getCooldown() / m_skillManager.getCurrentSkill()->getCooldownMax();
    }
    else
    {
        info.cd = 1.0f;
    }
    
	// Get Mouse and Keyboard states for this frame
	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::Get().SetMode(ks.IsKeyDown(DirectX::Keyboard::LeftAlt) ? DirectX::Mouse::MODE_ABSOLUTE : DirectX::Mouse::MODE_RELATIVE); // !TEMP!
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();

	// Temp for testing
	if (ks.IsKeyDown(DirectX::Keyboard::B))
	{
		btTransform transform = getTransform();
		transform.setOrigin({0, 0, 0});
		getRigidBody()->setWorldTransform(transform);
		getRigidBody()->setLinearVelocity({ 0, 0, 0 });
		m_moveDir = {0, 0, 0};
		m_moveSpeed = 0.f;
	}

	// TEMP FREE MOVE
	static bool freeMove = false;
	if (ks.IsKeyDown(DirectX::Keyboard::N) && !freeMove)
	{
		m_charController->setGravity({ 0.f, 0.f, 0.f }); // remove gravity
		freeMove = true;
		printf("free move activated\n");
	}
	else if (ks.IsKeyDown(DirectX::Keyboard::M) && freeMove)	
	{
		m_charController->setGravity({ 0.f, -PLAYER_GRAVITY, 0.f });
		// reset movement
		m_moveDir = { 0, 0, 0 };
		m_moveSpeed = 0.f;
		freeMove = false;
		printf("free move deactivated\n");
	}
		
	if (m_charController->onGround())
		m_playerState = PlayerState::STANDING;
	else
		m_playerState = PlayerState::IN_AIR;

	// Movement
	if (!ks.IsKeyDown(DirectX::Keyboard::LeftAlt))	// !TEMP!
		mouseMovement(deltaTime, &ms);
	jump(deltaTime, &ks);

	// Get movement input
	moveInput(&ks);
	if (!freeMove)
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

	// Print player velocity
//	printf("velocity: %f\n", m_moveSpeed);

	//crouch(deltaTime);

	// Weapon swap
	if (!m_weaponManager.isSwitching())
	{
		if (ks.IsKeyDown(m_switchWeaponOne))
			m_weaponManager.switchWeapon(0);

		if (ks.IsKeyDown(m_switchWeaponTwo))
			m_weaponManager.switchWeapon(1);

		if (ks.IsKeyDown(m_switchWeaponThree))
			m_weaponManager.switchWeapon(2);
	}

	// Skill
	if (ks.IsKeyDown(m_useSkill) && m_skillManager.getCanBeUsed())
	{
		m_skillManager.useSkill(getForwardBT(), *this);
	}
	if (ks.IsKeyUp(m_useSkill) && !m_skillManager.getCanBeUsed())
	{
		m_skillManager.releaseSkill();
	}

	// Check if reloading
	if (!m_weaponManager.isReloading())
	{
		// Primary and secondary attack
		if (!m_weaponManager.isAttacking())
		{
			btVector3 pos = getPositionBT() + btVector3(m_forward.x, m_forward.y, m_forward.z);
			if ((ms.leftButton))
				m_weaponManager.usePrimary(pos, m_camYaw, m_camPitch, *this);
			else if (ms.rightButton)
				m_weaponManager.useSecondary(pos, m_camYaw, m_camPitch, *this);
		}

		// Reload
		if (ks.IsKeyDown(m_reloadWeapon))
			m_weaponManager.reloadWeapon();
	}

	// Update weapon and skills
	m_weaponManager.update(deltaTime);
	m_skillManager.update(deltaTime);
}

//fills the HUD info with wave info
void Player::updateWaveInfo(int wave, int enemiesRemaining, float timeRemaning)
{
    info.wave = wave;
    info.enemiesRemaining = enemiesRemaining;
    info.timeRemaining = timeRemaning;
}

void Player::moveInput(DirectX::Keyboard::State * ks)
{
	// Reset wish direction
	m_wishDir = btVector3(0, 0, 0);
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
	btTransform transform = m_charController->getGhostObject()->getWorldTransform();
	transform.setOrigin(transform.getOrigin() + (m_wishDir * m_moveMaxSpeed * 2.f * deltaTime));
	m_charController->getGhostObject()->setWorldTransform(transform);
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

	m_moveSpeed += acceleration * deltaTime;

	if (m_playerState != PlayerState::IN_AIR && !m_wishJump && m_moveSpeed > m_moveMaxSpeed)
		m_moveSpeed = m_moveMaxSpeed;

	// Apply moveDir and moveSpeed to plyer
	if(m_playerState != PlayerState::IN_AIR)
		m_charController->setVelocityForTimeInterval(m_moveDir * m_moveSpeed, deltaTime);
	else
		m_charController->setVelocityForTimeInterval((m_moveDir * m_moveSpeed) + (m_wishDir * PLAYER_MOVEMENT_AIRSTRAFE_SPEED), deltaTime);

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
			applyFriction(deltaTime, friction);
	}
	else if (lookMovedirection > 0.f && m_wishDirRight > 0.f)
	{
		if (lookMoveAngle > PLAYER_STRAFE_ANGLE)
			m_moveDir = (m_moveDir + forward) * 0.5f;
		else
			applyFriction(deltaTime, friction);
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
	m_camYaw	+= m_mouseSens * (ms->x * deltaTime);
	m_camPitch	-= m_mouseSens * (ms->y * deltaTime);

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

DirectX::SimpleMath::Matrix Player::getTransformMatrix() const
{
	// Making memory for a matrix
	float* m = new float[4 * 16];

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

void Player::render(Graphics::Renderer & renderer)
{
	// Drawing the actual player model (can be deleted later, cuz we don't need it, unless we expand to multiplayer)
//	Object::render(renderer);

	// Setting position of updated weapon and skill models
	m_weaponManager.setWeaponModel(getTransformMatrix(), m_forward);
	//	m_skillManager.setWeaponModel(getTransformMatrix(), m_forward);

	// Drawing the weapon model
	m_weaponManager.render(renderer);
	m_skillManager.render(renderer);
    renderer.fillHUDInfo(&info);
}

void Logic::Player::setMaxSpeed(float maxSpeed)
{
	m_moveMaxSpeed = maxSpeed;
}

DirectX::SimpleMath::Vector3 Player::getPosition() const
{
	return DirectX::SimpleMath::Vector3(m_charController->getGhostObject()->getWorldTransform().getOrigin());
}

btVector3 Logic::Player::getPositionBT() const
{
	return m_charController->getGhostObject()->getWorldTransform().getOrigin();
}

float Logic::Player::getMoveSpeed() const
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

DirectX::SimpleMath::Vector3 Player::getForward()
{
	return m_forward;
}

btVector3 Player::getMoveDirection()
{
	return m_moveDir;
}

ListenerData & Logic::Player::getListenerData()
{
	return m_listenerData;
}
