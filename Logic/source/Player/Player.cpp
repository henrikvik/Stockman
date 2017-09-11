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
	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();
	

	move(deltaTime, &ks);
	jump(deltaTime);
	crouch(deltaTime);
	mouseMovement(deltaTime, &ms);

	if (ks.IsKeyDown(m_switchWeaponOne))
		m_weaponManager.switchWeapon(0);

	if (ks.IsKeyDown(m_switchWeaponTwo))
		m_weaponManager.switchWeapon(1);

	if (ks.IsKeyDown(m_switchWeaponThree))
		m_weaponManager.switchWeapon(2);

	if (ks.IsKeyDown(m_useSkill))
		m_skillManager.useSkill();

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
