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
#include <Graphics\include\Structs.h>

#define PLAYER_STARTING_HP				3
#define PLAYER_MOUSE_SENSETIVITY		0.1f
#define PLAYER_MOVEMENT_MAX_SPEED		0.015f
#define PLAYER_MOVEMENT_ACCELERATION	0.0002f
#define PLAYER_MOVEMENT_AIRACCELERATION	0.005f
#define PLAYER_MOVEMENT_AIRSTRAFE_SPEED 0.004f
#define PLAYER_SPEED_LIMIT				0.04f
#define PLAYER_STRAFE_ANGLE				0.95f
#define PLAYER_FRICTION					20.f
#define PLAYER_AIR_FRICTION				1.f
#define PLAYER_JUMP_SPEED				1.2f
#define PLAYER_BHOP_TIMER				10.f
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
		Physics* m_physPtr;

		// UI States
        Graphics::HUDInfo info;
		int m_hp;

		// Movements
		PlayerState m_playerState;
		DirectX::SimpleMath::Vector3 m_forward;
		float m_moveMaxSpeed;
		btVector3 m_moveDir; // only 2 dimensional movement direction (x, z)
		float m_moveSpeed;
		float m_acceleration;
		float m_deacceleration;
		float m_airAcceleration;
		float m_jumpSpeed;

		bool m_wishJump;
		btVector3 m_wishDir;
		float m_wishDirForward;
		float m_wishDirRight;

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
		void moveInput(DirectX::Keyboard::State* ks);
		void moveFree(float deltaTime, DirectX::Keyboard::State* ks);
		void move(float deltaTime);
		void airMove(float deltaTime);
		void accelerate(float deltaTime, float acceleration);
		void applyFriction(float deltaTime, float friction);
		void applyAirFriction(float deltaTime, float friction);
		void jump(float deltaTime, DirectX::Keyboard::State* ks);
		void crouch(float deltaTime);
		void mouseMovement(float deltaTime, DirectX::Mouse::State* ms);

	public:
		Player(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent);
		~Player();

		void init(Physics* physics, ProjectileManager* projectileManager, GameTime* gameTime);
		void clear();
		void reset();
		void updateSpecific(float deltaTime);
        void updateWaveInfo(int wave, int enemiesRemaining, float timeRemaning);
		void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
		void onCollision(Projectile& other);
		void affect(int stacks, Effect const &effect, float deltaTime);
		void upgrade(Upgrade const &upgrade);
		void render(Graphics::Renderer& renderer); 
		void setMaxSpeed(float maxSpeed);

		void saveToFile();
		void readFromFile();

		void takeDamage(int damage);
		int getHP() const;

		float getMoveSpeed() const;
		void setMoveSpeed(float speed);
		void setMoveDirection(btVector3 moveDir);
		btVector3 getForwardBT();
		DirectX::SimpleMath::Vector3 getForward();
		btVector3 getMoveDirection();

		static btVector3 startPosition;
	};

}

#endif // !PLAYER_H
