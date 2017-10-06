#include "Player/Player.h"
#include <AI\EnemyTest.h>
#include <AI\Trigger.h>

using namespace Logic;

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

	// Stats
	m_hp = PLAYER_STARTING_HP;
    	info.hp = m_hp;
   	info.cuttleryAmmo[0] = 0;
   	info.cuttleryAmmo[1] = 0;
    	info.iceAmmo[0] = 0 ;
    	info.iceAmmo[1] = 0 ;
    	info.wave = 0;
    	info.score = 0;
    	info.sledge = false;

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
}

void Player::onCollision(Entity& other, btVector3 contactPoint, float dmgMultiplier)
{
	if (Projectile* p	= dynamic_cast<Projectile*>(&other))	onCollision(*p);										// collision with projectile
	else if (Trigger* t = dynamic_cast<Trigger*>(&other))		{ }														// collision with trigger
	else if(m_playerState == PlayerState::IN_AIR)
	{
		btVector3 dir = contactPoint - getPositionBT();

		btVector3 hitSurfaceNormal = m_physPtr->RayTestGetNormal(Ray(getPositionBT(), getPositionBT() + (dir*2))); // overshoot the ray test to get correct result

		if (!hitSurfaceNormal.isZero())
		{
			float hitAngle = hitSurfaceNormal.dot({ 0.f, 1.f, 0.f });

			// if angle between up-vector and surface-vector is over 0.8 player is grounded and can jump again
			if (hitAngle > 0.8f)
				m_playerState = PlayerState::STANDING;
			else
				m_playerState = PlayerState::IN_AIR;
		}
	}
		
}

void Player::onCollision(Projectile& other)
{
	if (other.getProjectileData().enemyBullet)
		takeDamage(other.getProjectileData().damage);
}

void Player::affect(int stacks, Effect const & effect, float deltaTime)
{
	long long flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_MODIFY_MOVEMENTSPEED)
	{
		getRigidbody()->setLinearVelocity(btVector3(getRigidbody()->getLinearVelocity().x(), 0, getRigidbody()->getLinearVelocity().z()));
		getRigidbody()->applyCentralImpulse(btVector3(0, 1500.f * stacks, 0));
		m_playerState = PlayerState::STANDING;
	}

	if (flags & Effect::EFFECT_MODIFY_AMMO)
	{
		printf("Ammo pack!\n");
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

void Player::takeDamage(int damage)
{
	m_hp -= damage;
}

int Player::getHP() const
{
	return m_hp;
}

void Player::updateSpecific(float deltaTime)
{
    //updates hudInfo with the current info
    info.hp = m_hp;
    info.cuttleryAmmo[0] = m_weaponManager.getfirstWeapon()->getMagAmmo();
    info.cuttleryAmmo[1] = m_weaponManager.getfirstWeapon()->getMagSize();
    info.iceAmmo[0] = m_weaponManager.getSecondWeapon()->getMagAmmo();
    info.iceAmmo[1] = m_weaponManager.getSecondWeapon()->getMagSize();
    if (m_weaponManager.getCurrentWeaponPrimary()->getMagSize() == 0)
    {
        info.sledge  = true;
    }
    else
    {
        info.sledge = false;
    }

	// Get Mouse and Keyboard states for this frame
	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::Get().SetMode(ks.IsKeyDown(DirectX::Keyboard::LeftAlt) ? DirectX::Mouse::MODE_ABSOLUTE : DirectX::Mouse::MODE_RELATIVE); // !TEMP!
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();

	// Temp for testing
	if (ks.IsKeyDown(DirectX::Keyboard::B))
	{
		btTransform transform = getRigidbody()->getWorldTransform();
		transform.setOrigin({0, 0, 0});
		getRigidbody()->setWorldTransform(transform);
		getRigidbody()->setLinearVelocity({ 0, 0, 0 });
		m_moveDir = {0, 0, 0};
		m_moveSpeed = 0.f;
	}

	// TEMP FREE MOVE
	static bool freeMove = false;
	if (ks.IsKeyDown(DirectX::Keyboard::N) && !freeMove)
	{
		getRigidbody()->setGravity({ 0.f, 0.f, 0.f }); // remove gravity
		freeMove = true;
		printf("free move activated\n");
	}
	else if (ks.IsKeyDown(DirectX::Keyboard::M) && freeMove)
	{
		// reset movement
		getRigidbody()->setGravity({ 0.f, -PHYSICS_GRAVITY, 0.f });
		getRigidbody()->setLinearVelocity({ 0, 0, 0 });
		m_moveDir = { 0, 0, 0 };
		m_moveSpeed = 0.f;
		freeMove = false;
		printf("free move deactivated\n");
	}
		

	// Movement
	if (!ks.IsKeyDown(DirectX::Keyboard::LeftAlt))	// !TEMP!
		mouseMovement(deltaTime, &ms);
	jump(deltaTime, &ks);

	// If moving on y-axis, player is in air
	if (!m_wishJump && (getRigidbody()->getLinearVelocity().y() > 1.f || getRigidbody()->getLinearVelocity().y() < -1.f))
		m_playerState = PlayerState::IN_AIR;

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
	if(!m_wishDir.isZero())
		m_wishDir = m_wishDir.safeNormalize();
}

void Player::moveFree(float deltaTime, DirectX::Keyboard::State * ks)
{
	getRigidbody()->setLinearVelocity({ 0, 0, 0 }); // Remove linear velocity

	if (ks->IsKeyDown(DirectX::Keyboard::Keys::LeftControl)) // down
		m_wishDir.setY(-1.f);
	if (ks->IsKeyDown(DirectX::Keyboard::Keys::Space))		 // up
		m_wishDir.setY(1.f);

	// Update pos of player
	btTransform transform = getRigidbody()->getWorldTransform();
	transform.setOrigin(getRigidbody()->getWorldTransform().getOrigin() + (m_wishDir * m_moveMaxSpeed * 2.f * deltaTime));
	getRigidbody()->setWorldTransform(transform);
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
		getRigidbody()->setLinearVelocity({ 0, PLAYER_JUMP_SPEED, 0 }); // Jump
		m_playerState = PlayerState::IN_AIR;

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
	m_moveSpeed += acceleration * deltaTime;

	if (m_playerState != PlayerState::IN_AIR && !m_wishJump && m_moveSpeed > m_moveMaxSpeed)
		m_moveSpeed = m_moveMaxSpeed;

	// Update pos of player
	
	btTransform transform = getRigidbody()->getWorldTransform();
	if (m_playerState != PlayerState::IN_AIR)
		transform.setOrigin(getRigidbody()->getWorldTransform().getOrigin() + (m_moveDir * m_moveSpeed * deltaTime));
	else
		transform.setOrigin(getRigidbody()->getWorldTransform().getOrigin() + (m_moveDir * m_moveSpeed * deltaTime) + (m_wishDir * PLAYER_MOVEMENT_AIRSTRAFE_SPEED * deltaTime));
	getRigidbody()->setWorldTransform(transform);
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
	m_camYaw	+= m_mouseSens * (ms->x);
	m_camPitch	-= m_mouseSens * (ms->y);

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
