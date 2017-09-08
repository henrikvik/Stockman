#include "Player/Player.h"

using namespace Logic;

Player::Player()
{
}

Player::~Player()
{
}

bool Logic::Player::init()
{

	// Default controlls
	m_moveLeft = DirectX::Keyboard::Keys::A;
	m_moveRight = DirectX::Keyboard::Keys::D;
	m_moveForward = DirectX::Keyboard::Keys::W;
	m_moveBack = DirectX::Keyboard::Keys::S;
	m_jump = DirectX::Keyboard::Keys::Space;
	m_switchWeaponPrimary = DirectX::Keyboard::Keys::D1;
	m_switchWeaponSecondary = DirectX::Keyboard::Keys::D2;
	m_switchWeaponMelee = DirectX::Keyboard::Keys::D3;
	m_useSkill = DirectX::Keyboard::Keys::E;

	return true;
}

void Player::clear()
{
}

void Player::onCollision(Entity& other)
{
}

void Player::updateSpecific(float deltaTime)
{
	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();

	move(deltaTime, &ks);
	jump(deltaTime);
	crouch(deltaTime);

	if (ks.IsKeyDown(m_switchWeaponPrimary))
		// m_actionManager.switchWeapon(primary);
		false;

	if (ks.IsKeyDown(m_switchWeaponSecondary))
		// m_actionManager.switchWeapon(secondary);
		false;

	if (ks.IsKeyDown(m_switchWeaponMelee))
		// m_actionManager.switchWeapon(melee);
		false;

	if (ks.IsKeyDown(m_useSkill))
		// m_actionManager.useSkill();
		false;

	if ((ms.leftButton))
		// m_actionManager.firePrimary();
		false;

	if (ms.rightButton)
		// m_actionManager.fireSecondary();
		false;

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