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
#include <Keyboard.h>
#include <Mouse.h>

#include "Entity\Entity.h"
#include "Weapon\WeaponManager.h"
#include "Skill\SkillManager.h"

namespace Logic
{
	class Player : public Entity
	{
	private:
		//ActionManager m_actionManager;
		WeaponManager m_weaponManager;
		SkillManager m_skillManager;

		// Stats
		float m_mouseSens;
		DirectX::SimpleMath::Vector3 m_lookAt;
		float m_moveSpeed;

		// Keys
		DirectX::Keyboard::Keys m_keyLeft;
		DirectX::Keyboard::Keys m_moveRight;
		DirectX::Keyboard::Keys m_moveLeft;
		DirectX::Keyboard::Keys m_moveForward;
		DirectX::Keyboard::Keys m_moveBack;
		DirectX::Keyboard::Keys m_jump;
		DirectX::Keyboard::Keys m_crouch;
		DirectX::Keyboard::Keys m_switchWeaponOne;
		DirectX::Keyboard::Keys m_switchWeaponTwo;
		DirectX::Keyboard::Keys m_switchWeaponThree;
		DirectX::Keyboard::Keys m_reloadWeapon;
		DirectX::Keyboard::Keys m_useSkill;

		// Movement
		void move(float deltaTime, DirectX::Keyboard::State* ks);
		void jump(float deltaTime);
		void crouch(float deltaTime);
		void mouseMovement(float deltaTime, DirectX::Mouse::State* ms);

		DirectX::SimpleMath::Vector2 getWindowMidPoint();

	public:
		Player();
		~Player();

		bool init(Physics* physics, BodyDesc bodyDesc);
		void clear();
		void updateSpecific(float deltaTime);
		void onCollision(Entity& other);

		void saveToFile();
		void readFromFile();
	};

}

#endif // !PLAYER_H
