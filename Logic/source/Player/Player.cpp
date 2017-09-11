#include "Player/Player.h"

using namespace Logic;

Player::Player()
{
}

Player::~Player()
{
}

bool Player::init(Physics* physics, RigidBodyDesc rigidBodyDesc)
{
	Entity::init(physics, rigidBodyDesc);
	m_weaponManager.init();
	m_skillManager.init();

	// Default controlls
	m_moveLeft = DirectX::Keyboard::Keys::A;
	m_moveRight = DirectX::Keyboard::Keys::D;
	m_moveForward = DirectX::Keyboard::Keys::W;
	m_moveBack = DirectX::Keyboard::Keys::S;
	m_jump = DirectX::Keyboard::Keys::Space;
	m_switchWeaponOne = DirectX::Keyboard::Keys::D1;
	m_switchWeaponTwo = DirectX::Keyboard::Keys::D2;
	m_switchWeaponThree = DirectX::Keyboard::Keys::D3;
	m_useSkill = DirectX::Keyboard::Keys::E;

	return true;
}

void Player::clear()
{
}

void Player::onCollision(Entity& other)
{
}

void Logic::Player::saveToFile()
{
}

void Logic::Player::readFromFile()
{
}

void Player::updateSpecific(float deltaTime)
{
	// Update Managers
	m_weaponManager.update(deltaTime);
	m_skillManager.update();

	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();
	HWND hwnd = FindWindow(NULL, "Stort spel");

	RECT rect;
	GetWindowRect(hwnd, &rect);

	printf("bot: %d	left: %d	right: %d	top: %d\n", rect.bottom, rect.left, rect.right, rect.top);

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

	// Skill
	if (ks.IsKeyDown(m_useSkill))
		m_skillManager.useSkill();

	// Primary and secondary attack
	if ((ms.leftButton))
		m_weaponManager.usePrimary();

	if (ms.rightButton)
		m_weaponManager.useSecondary();

}

void Logic::Player::move(float deltaTime, DirectX::Keyboard::State* ks)
{
	if (ks->IsKeyDown(m_moveLeft))
		// move player left
		false;

	if (ks->IsKeyDown(m_moveRight))
		// move player right
		false;

	if (ks->IsKeyDown(m_moveForward))
		// move player forward
		false;

	if (ks->IsKeyDown(m_moveBack))
		// move player back
		false;
}

void Logic::Player::jump(float deltaTime)
{
	// jump
}

void Logic::Player::crouch(float deltaTime)
{
	// crouch
}

void Logic::Player::mouseMovement(float deltaTime, DirectX::Mouse::State * ms)
{

}
