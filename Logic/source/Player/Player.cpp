#include "Player/Player.h"
#include <AI\EnemyTest.h>

using namespace Logic;

Player::Player(btRigidBody* body, btVector3 halfExtent)
: Entity(body, halfExtent)
{

}

Player::~Player()
{
	clear();
}

void Player::init(ProjectileManager* projectileManager, GameTime* gameTime)
{
	m_weaponManager.init(projectileManager);
	m_skillManager.init(projectileManager, gameTime);

	// Default mouse sensetivity, lookAt
	m_camYaw = 90;
	m_camPitch = 5;

	m_playerState = PlayerState::STANDING;
	m_mouseSens = PLAYER_MOUSE_SENSETIVITY;
	m_forward = DirectX::SimpleMath::Vector3(0, 0, 1);
	m_moveMaxSpeed = PLAYER_MOVEMENT_MAX_SPEED;
	m_moveDir = btVector3(1, 1, 1);
	m_moveSpeed = 0.f;
	m_acceleration = PLAYER_MOVEMENT_ACCELERATION;
	m_jumpSpeed = PLAYER_JUMP_SPEED;

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
	//m_skillManager.clear();
}

void Player::onCollision(Entity& other)
{
	if (Projectile* p	= dynamic_cast<Projectile*>(&other))	onCollision(*p);
	else if (EnemyTest* e = dynamic_cast<EnemyTest*>(&other))
	{
		printf("Enemy slapped you right in the face.\n");
	}
	else
	{
		m_playerState = PlayerState::STANDING; // TEMP
	}
}

void Player::onCollision(Projectile& other)
{

}

void Player::affect(int stacks, Effect const & effect, float deltaTime)
{
	int flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_MODIFY_MOVEMENTSPEED)
	{
		getRigidbody()->setLinearVelocity(btVector3(getRigidbody()->getLinearVelocity().x(), 0, getRigidbody()->getLinearVelocity().z()));
		getRigidbody()->applyCentralImpulse(btVector3(0, 1500.f * stacks, 0));
		m_playerState = PlayerState::IN_AIR;
	}
}

void Player::saveToFile()
{

}

void Player::readFromFile()
{

}

void Player::updateSpecific(float deltaTime)
{
	// Get Mouse and Keyboard states for this frame
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);
	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();

	// Movement
	mouseMovement(deltaTime, &ms);
	move(deltaTime, &ks);
	jump(deltaTime, &ks);
	crouch(deltaTime);

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

	// Check if reloading
	if (!m_weaponManager.isReloading())
	{
		// Skill
		if (ks.IsKeyDown(m_useSkill))
			m_skillManager.useSkill();

		// Primary and secondary attack
		if (!m_weaponManager.isAttacking())
		{
			btVector3 pos = getPositionBT() + btVector3(m_forward.x, m_forward.y, m_forward.z) * 2.85f;
			if ((ms.leftButton))
				m_weaponManager.usePrimary(pos, m_camYaw, m_camPitch);
			else if (ms.rightButton)
				m_weaponManager.useSecondary(pos, m_camYaw, m_camPitch);
		}

		// Reload
		if (ks.IsKeyDown(m_reloadWeapon))
			m_weaponManager.reloadWeapon();
	}

	// Update weapon and skills
	m_weaponManager.update(deltaTime);
	m_skillManager.update(deltaTime);

	m_weaponManager.setWeaponModel(getTransformMatrix(), m_forward);
	//	m_skillManager.setWeaponModel(getTransformMatrix(), m_forward);
}

void Player::move(float deltaTime, DirectX::Keyboard::State* ks)
{
	bool nonePressed = true;

	btVector3 accelDir = btVector3(0, 0, 0);

	// Move Left
	if (ks->IsKeyDown(m_moveLeft))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).cross(btVector3(0, 1, 0)).normalize();
		accelDir += -dir;
		nonePressed = false;
	}

	// Move Right
	if (ks->IsKeyDown(m_moveRight))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).cross(btVector3(0, 1, 0)).normalize();
		accelDir += dir;
		nonePressed = false;
	}

	// Move Forward
	if (ks->IsKeyDown(m_moveForward))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).normalize();
		accelDir += dir;
		nonePressed = false;
	}

	// Move Back
	if (ks->IsKeyDown(m_moveBack))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).normalize();
		accelDir += -dir;
		nonePressed = false;
	}

	// Air movement TEMP
	if (m_playerState == PlayerState::IN_AIR)
	{
		m_moveDir += m_moveDir * accelDir.dot(btVector3(m_forward.x, 0.f, m_forward.z));
	}
	else
		m_moveDir += accelDir;
	
	// Normalize movement direction
	m_moveDir *= 1000;
	if (m_moveDir.getX() != 0.f || m_moveDir.getY() != 0.f || m_moveDir.getZ() != 0.f)
		m_moveDir = btVector3(m_moveDir).normalize();

	// If no movement key is pressed set reduce movespeed for brake effect
	if (nonePressed)
	{
		float toBrake = m_acceleration * deltaTime;

		if (m_moveSpeed < toBrake)
			m_moveSpeed = 0.f;
		else if(m_moveSpeed > toBrake)
			m_moveSpeed -= toBrake;
	}
	// If any movement key is pressed add acceleration
	else
	{
		m_moveSpeed += m_acceleration * deltaTime;
		
		if (m_moveSpeed > m_moveMaxSpeed)
			m_moveSpeed = m_moveMaxSpeed;
	}

	// Update pos from movement direction and speed
	btTransform transform = getRigidbody()->getWorldTransform();
	transform.setOrigin(getRigidbody()->getWorldTransform().getOrigin() + m_moveDir * m_moveSpeed);
	getRigidbody()->setWorldTransform(transform);
}

void Player::jump(float deltaTime, DirectX::Keyboard::State* ks)
{
	if (ks->IsKeyDown(m_jump) && m_playerState != PlayerState::IN_AIR)
	{
		getRigidbody()->applyCentralImpulse(btVector3(0, m_jumpSpeed, 0));
		m_playerState = PlayerState::IN_AIR;
	}
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

	// Drawing the weapon model
	m_weaponManager.render(renderer);
}

DirectX::SimpleMath::Vector3 Player::getForward()
{
	return m_forward;
}