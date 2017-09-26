#ifndef PLAYER_H
#define PLAYER_H

#pragma region ClassDesc
		/*
			CLASS: Player
			AUTHOR:

			DESCRIPTION: Handles input-data from user
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
#define PLAYER_MOVEMENT_MAX_SPEED		0.1f
#define PLAYER_MOVEMENT_ACCELERATION	0.005f
#define PLAYER_JUMP_SPEED				50.f
#define PLAYER_MOVEMENT_HORIZONTAL_CAP	20.f
#define PLAYER_MOVEMENT_VERTICAL_CAP	100.f

namespace Logic
{
	class Player : public Entity
	{
	private:

		enum PlayerState
		{
			STANDING,
			CROUCHING,
			IN_AIR
		};

		//ActionManager m_actionManager;
		WeaponManager m_weaponManager;
		SkillManager m_skillManager;

		// Stats
		PlayerState m_playerState;
		DirectX::SimpleMath::Vector3 m_forward;
		float m_moveMaxSpeed;
		btVector3 m_moveDir;
		float m_moveSpeed;
		float m_acceleration;
		float m_jumpSpeed;

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
		void jump(float deltaTime, DirectX::Keyboard::State* ks);
		void crouch(float deltaTime);
		void mouseMovement(float deltaTime, DirectX::Mouse::State* ms);

	public:
		Player(btRigidBody* body, btVector3 halfExtent);
		~Player();

		void init(ProjectileManager* projectileManager, GameTime* gameTime);
		void clear();
		void updateSpecific(float deltaTime);
		void onCollision(Entity& other);
		void onCollision(Projectile& other);
		void affect(int stacks, Effect const &effect, float deltaTime);
		void render(Graphics::Renderer& renderer); 

		void saveToFile();
		void readFromFile();

		DirectX::SimpleMath::Vector3 getForward();
	};

}

#endif // !PLAYER_H
