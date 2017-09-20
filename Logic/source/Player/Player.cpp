#include "Player/Player.h"

using namespace Logic;

Player::Player(btRigidBody* body)
: Entity(body)
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

	m_mouseSens = PLAYER_MOUSE_SENSETIVITY;
	m_forward = DirectX::SimpleMath::Vector3(0, 0, 1);
	m_moveSpeed = PLAYER_MOVEMENT_SPEED;

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
}

void Player::onCollision(Entity& other)
{
	if (Projectile* p = dynamic_cast<Projectile*>(&other))
	{
		onCollision(*p);
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
	m_skillManager.update();

	// Get Mouse and Keyboard states for this frame
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);
	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();

	// Movement
	mouseMovement(deltaTime, &ms);
	move(deltaTime, &ks);
	jump(deltaTime);
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
	btRigidBody* rigidBody = getRigidbody();

	btVector3 linearVel = btVector3(0, 0, 0);
	// Move Left
	if (ks->IsKeyDown(m_moveLeft))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).cross(btVector3(0, 1, 0)).normalize();
		linearVel += -dir;
	}

	// Move Right
	if (ks->IsKeyDown(m_moveRight))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).cross(btVector3(0, 1, 0)).normalize();
		linearVel += dir;
	}

	// Move Forward
	if (ks->IsKeyDown(m_moveForward))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).normalize();
		linearVel += dir;
	}

	// Move Back
	if (ks->IsKeyDown(m_moveBack))
	{
		btVector3 dir = btVector3(m_forward.x, 0, m_forward.z).normalize();
		linearVel += -dir;
	}

	// Apply final force
	rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + linearVel * deltaTime * m_moveSpeed);

	// Setting movement caps
	btVector3 lv = rigidBody->getLinearVelocity();
	float x = lv.getX(), y = lv.getY(), z = lv.getZ();
	float hcap = PLAYER_MOVEMENT_HORIZONTAL_CAP;
	float vcap = PLAYER_MOVEMENT_VERTICAL_CAP;
	if (x > hcap || x < -hcap) rigidBody->setLinearVelocity(btVector3((x > 0) ? hcap : -hcap, y, z));
	if (y > vcap || y < -vcap) rigidBody->setLinearVelocity(btVector3(x, (y > 0) ? vcap : -vcap, z));
	if (z > hcap || z < -hcap) rigidBody->setLinearVelocity(btVector3(x, y, (z > 0) ? hcap : -hcap));
}

void Player::jump(float deltaTime)
{
	// jump
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