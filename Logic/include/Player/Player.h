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
#include <Projectile\ProjectileManager.h>

#define PLAYER_MOUSE_SENSETIVITY		0.1f
#define PLAYER_MOVEMENT_SPEED			0.1f
#define PLAYER_MOVEMENT_HORIZONTAL_CAP	20.f
#define PLAYER_MOVEMENT_VERTICAL_CAP	100.f

namespace Logic
{
	class Player : public Entity
	{
	private:
		//ActionManager m_actionManager;
		WeaponManager m_weaponManager;
		SkillManager m_skillManager;

		// Stats
		DirectX::SimpleMath::Vector3 m_forward;
		float m_moveSpeed;

		// Mouse
		float m_mouseSens;
		float m_camYaw;
		float m_camPitch;

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
		Player(btRigidBody* body, btVector3 halfExtent);
		~Player();

		void init(ProjectileManager* projectileManager);
		void clear();
		void updateSpecific(float deltaTime);
		void onCollision(Entity& other);
		void onCollision(Projectile& other);

		void saveToFile();
		void readFromFile();

		DirectX::SimpleMath::Vector3 getForward();
	};

}

#endif // !PLAYER_H
