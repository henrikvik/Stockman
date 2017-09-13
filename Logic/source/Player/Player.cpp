#include "Player/Player.h"

using namespace Logic;

Player::Player()
{
}

Player::~Player()
{
	clear();
}

void Player::init(Physics* physics, BodyDesc bodyDesc)
{
	Entity::init(physics, bodyDesc);

	m_weaponManager.init();
	m_skillManager.init();

	// Default mouse sensetivity, lookAt
	m_mouseSens = 1.f;
	m_lookAt = DirectX::SimpleMath::Vector3(0, 0, 1);

	m_moveSpeed = 1.f;

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
//	m_skillManager.clear();
}

void Player::onCollision(Entity& other)
{
}

DirectX::SimpleMath::Vector3 Logic::Player::getLookAt()
{
	return m_lookAt;
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
	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();

	// Movement
	if(ks.IsKeyDown(DirectX::Keyboard::X))
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
				m_weaponManager.usePrimary();

			if (ms.rightButton)
				m_weaponManager.useSecondary();
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
		btVector3 dir = btVector3(m_lookAt.x, 0, m_lookAt.z).cross(btVector3(0, 1, 0)).normalize();
		linearVel += dir;
	}

	// Move Right
	if (ks->IsKeyDown(m_moveRight))
	{
		btVector3 dir = btVector3(m_lookAt.x, 0, m_lookAt.z).cross(btVector3(0, 1, 0)).normalize();
		linearVel += -dir;
	}

	// Move Forward
	if (ks->IsKeyDown(m_moveForward))
	{
		btVector3 dir = btVector3(m_lookAt.x, 0, m_lookAt.z).normalize();
		linearVel += dir;
	}

	// Move Back
	if (ks->IsKeyDown(m_moveBack))
	{
		btVector3 dir = btVector3(m_lookAt.x, 0, m_lookAt.z).normalize();
		linearVel += -dir;
	}

	// Apply final force
	rigidBody->applyCentralForce(linearVel * deltaTime * m_moveSpeed);

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

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	static float yaw;
	static float pitch;

	if (deltaTime < 0.1f) // TEMP, fix for EXTREMELY low delta time
		deltaTime = 0.1f;

	yaw -= deltaTime * m_mouseSens * (cursorPos.x - midPoint.x);
	pitch -= deltaTime * m_mouseSens * (cursorPos.y - midPoint.y);

	//printf("x: %f  y: %f\n", cursorPos.x - midPoint.x, cursorPos.y - midPoint.y);
	//printf("x: %f  y: %f\n", yaw, pitch);
	//printf("msx: %d	msy: %d\n", cursorPos.x, cursorPos.y);

	// Pitch lock and yaw correction
	if (pitch > 89)
		pitch = 89;
	if (pitch < -89)
		pitch = -89;
	if (yaw < 0.f)
		yaw += 360.f;
	if (yaw > 360.f)
		yaw -= 360.f;

	// Reset cursor to mid point of window
	SetCursorPos(midPoint.x, midPoint.y);

	// Create lookAt
	m_lookAt.x = cos(DirectX::XMConvertToRadians(pitch)) * cos(DirectX::XMConvertToRadians(yaw));
	m_lookAt.y = sin(DirectX::XMConvertToRadians(pitch));
	m_lookAt.z = cos(DirectX::XMConvertToRadians(pitch)) * sin(DirectX::XMConvertToRadians(yaw));

	m_lookAt.Normalize();

	//printf("x: %f  y: %f  z: %f\n", m_lookAt.x, m_lookAt.y, m_lookAt.z);
}

DirectX::SimpleMath::Vector2 Logic::Player::getWindowMidPoint()
{
	HWND hwnd = FindWindow(NULL, "Stort spel");

	RECT rect;
	GetWindowRect(hwnd, &rect);

	return DirectX::SimpleMath::Vector2((rect.left + rect.right) * 0.5f, (rect.top + rect.bottom) * 0.5f); // Returns mid point for window
}
