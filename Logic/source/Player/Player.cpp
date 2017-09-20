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

void Player::init(ProjectileManager* projectileManager)
{
	m_weaponManager.init(projectileManager);
	m_skillManager.init();

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

void Player::saveToFile()
{

}

void Player::readFromFile()
{

}

void Player::updateSpecific(float deltaTime)
{
	// Update Managers
	m_weaponManager.update(deltaTime);
	//m_skillManager.update(deltaTime);

	// Get Mouse and Keyboard states for this frame
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);
	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();

	// Movement
	mouseMovement(deltaTime, &ms);
	move(deltaTime, &ks);
	/*if (ks.IsKeyDown(DirectX::Keyboard::Keys::X))
	{
		btTransform transform;
		getRigidbody()->getMotionState()->getWorldTransform(transform);
		transform.setOrigin(getRigidbody()->getWorldTransform().getOrigin() + btVector3(0.01f, 0, 0));
		getRigidbody()->getMotionState()->setWorldTransform(transform);
	}*/
	
	if(ks.IsKeyDown(m_jump))
		jump();
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
			if ((ms.leftButton))
				m_weaponManager.usePrimary(getPositionBT(), m_camYaw, m_camPitch);
			else if (ms.rightButton)
				m_weaponManager.useSecondary(getPositionBT(), m_camYaw, m_camPitch);
		}

		// Reload
		if (ks.IsKeyDown(m_reloadWeapon))
			m_weaponManager.reloadWeapon();
	}
}

void Player::move(float deltaTime, DirectX::Keyboard::State* ks)
{
	bool nonePressed = true;

	// Move Left
	if (ks->IsKeyDown(m_moveLeft))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).cross(btVector3(0, 1, 0)).normalize();
		m_moveDir += -dir;
		nonePressed = false;
	}

	// Move Right
	if (ks->IsKeyDown(m_moveRight))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).cross(btVector3(0, 1, 0)).normalize();
		m_moveDir += dir;
		nonePressed = false;
	}

	// Move Forward
	if (ks->IsKeyDown(m_moveForward))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).normalize();
		m_moveDir += dir;
		nonePressed = false;
	}

	// Move Back
	if (ks->IsKeyDown(m_moveBack))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).normalize();
		m_moveDir += -dir;
		nonePressed = false;
	}

	m_moveDir *= 1000;
	if (m_moveDir.getX() != 0.f || m_moveDir.getY() != 0.f || m_moveDir.getZ() != 0.f)
		m_moveDir = btVector3(m_moveDir).normalize();

	if (nonePressed)
	{
		// -accel
		float toBrake = m_acceleration * deltaTime;

		if (m_moveSpeed < toBrake)
			m_moveSpeed = 0.f;
		else if(m_moveSpeed > toBrake)
			m_moveSpeed -= toBrake;
	}
	else
	{
		m_moveSpeed += m_acceleration * deltaTime;
		
		if (m_moveSpeed > m_moveMaxSpeed)
			m_moveSpeed = m_moveMaxSpeed;
		else if (m_moveSpeed < m_moveMaxSpeed)
			m_moveSpeed += m_moveMaxSpeed;
	}

	btTransform transform;
	getRigidbody()->getMotionState()->getWorldTransform(transform);
	transform.setOrigin(getRigidbody()->getWorldTransform().getOrigin() + m_moveDir * m_moveSpeed);
	getRigidbody()->getMotionState()->setWorldTransform(transform);

	// Apply final force
	//rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + linearVel * deltaTime * m_moveSpeed);

	// Setting movement caps
	/*btVector3 lv = rigidBody->getLinearVelocity();
	float x = lv.getX(), y = lv.getY(), z = lv.getZ();
	float hcap = PLAYER_MOVEMENT_HORIZONTAL_CAP;
	float vcap = PLAYER_MOVEMENT_VERTICAL_CAP;
	if (x > hcap || x < -hcap) rigidBody->setLinearVelocity(btVector3((x > 0) ? hcap : -hcap, y, z));
	if (y > vcap || y < -vcap) rigidBody->setLinearVelocity(btVector3(x, (y > 0) ? vcap : -vcap, z));
	if (z > hcap || z < -hcap) rigidBody->setLinearVelocity(btVector3(x, y, (z > 0) ? hcap : -hcap));*/
}

void Player::jump()
{
	// jump
	if (m_playerState != PlayerState::IN_AIR)
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
	DirectX::SimpleMath::Vector2 midPoint = getWindowMidPoint();

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

DirectX::SimpleMath::Vector2 Logic::Player::getWindowMidPoint()
{
	HWND hwnd = FindWindow(NULL, "Stort spel");

	RECT rect;
	GetWindowRect(hwnd, &rect);

	return DirectX::SimpleMath::Vector2((rect.left + rect.right) * 0.5f, (rect.top + rect.bottom) * 0.5f); // Returns mid point for window
}

DirectX::SimpleMath::Vector3 Player::getForward()
{
	return m_forward;
}