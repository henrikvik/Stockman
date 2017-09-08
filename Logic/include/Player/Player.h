#ifndef PLAYER_H
#define PLAYER_H
#pragma region ClassDesc
		/*
			CLASS: WeaponManager
			AUTHOR:

			DESCRIPTION: TODO
		*/
#pragma endregion

#include <Windows.h>
#include "Keyboard.h"

#include "Entity\Entity.h"
#include "ActionManager.h"

namespace Logic
{
	class Player : public Entity
	{
	private:
		ActionManager m_actionManager;
		DirectX::Keyboard* m_keyboard;

		// Keys
		DirectX::Keyboard::Keys m_keyoveLeft;
		DirectX::Keyboard::Keys m_moveRight;
		DirectX::Keyboard::Keys m_moveLeft;
		DirectX::Keyboard::Keys m_moveForward;
		DirectX::Keyboard::Keys m_moveBack;
		DirectX::Keyboard::Keys m_jump;
		DirectX::Keyboard::Keys m_crouch;
		DirectX::Keyboard::Keys m_switchWeaponPrimary;
		DirectX::Keyboard::Keys m_switchWeaponSecondary;
		DirectX::Keyboard::Keys m_switchWeaponMelee;
		DirectX::Keyboard::Keys m_useSkill;
		DirectX::Keyboard::Keys m_firePrimary;
		DirectX::Keyboard::Keys m_fireSecondary;

		// Movement
		void move(float deltaTime, DirectX::Keyboard::State* ks);
		void jump(float deltaTime);
		void crouch(float deltaTime);

	public:
		Player();
		~Player();

		bool init(DirectX::Keyboard* mKeyboard);
		void clear();
		void updateSpecific(float deltaTime);
		void onCollision(const Entity& other);
	};

}

#endif // !PLAYER_H
