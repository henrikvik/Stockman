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
#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

#define PLAYER_GRAVITY					9.82f * 2.f * 0.0000015f
#define PLAYER_SIZE_RADIUS				0.5f
#define PLAYER_SIZE_HEIGHT				2.f
#define PLAYER_STARTING_HP				3
#define PLAYER_MOUSE_SENSETIVITY		0.01f
#define PLAYER_MOVEMENT_MAX_SPEED		0.015f
#define PLAYER_MOVEMENT_ACCELERATION	0.0002f
#define PLAYER_MOVEMENT_AIRACCELERATION	0.005f
#define PLAYER_MOVEMENT_AIRSTRAFE_SPEED 0.004f
#define PLAYER_SPEED_LIMIT				0.04f
#define PLAYER_STRAFE_ANGLE				0.95f
#define PLAYER_FRICTION					20.f
#define PLAYER_AIR_FRICTION				1.f
#define PLAYER_JUMP_SPEED				0.008f

namespace Sound
{
    struct ListenerData;
}

namespace Logic
{
    class Physics;
    class WeaponManager;
    class Weapon;
    class SkillManager;
    class Skill;
    class ProjectileManager;

	class Player : public Entity
	{
	public:
		enum PlayerState
		{
			STANDING,
			CROUCHING,
			IN_AIR
		};

	private:
        // Special modes (move to other class)
        bool m_godMode, m_noclip;

		btKinematicCharacterController* m_charController;

		//ActionManager m_actionManager;
		WeaponManager* m_weaponManager;
		SkillManager* m_skillManager;
		Physics* m_physPtr;

		// UI States
		int m_hp;
        int currentWeapon;
        int currentSkills[2];

		// Movements
		PlayerState m_playerState;
		DirectX::SimpleMath::Vector3 m_forward;
		float m_moveMaxSpeed;
		btVector3 m_moveDir; // only 2 dimensional movement direction (x, z)
		float m_moveSpeed;
        float m_moveSpeedMod;
        //TODO LUKAS SWITCH THIS OUT AS PROMISED
        float m_permanentSpeedMod;
        //TODO LUKAS SWITCH THIS OUT AS PROMISED
		float m_acceleration;
		float m_deacceleration;
		float m_airAcceleration;
		float m_jumpSpeed;

		bool m_wishJump;
		btVector3 m_wishDir;
		float m_wishDirForward;
		float m_wishDirRight;

		// Sound
		Sound::ListenerData* m_listenerData;

		// Mouse
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
		DirectX::Keyboard::Keys m_useSkillPrimary;
        DirectX::Keyboard::Keys m_useSkillSecondary;
        DirectX::Keyboard::Keys m_useSkillTertiary;

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

		// Sound
		void updateSound(float deltaTime);

        //status
        bool m_stunned;
	public:
		Player(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent);
		~Player();

        void registerDebugCmds();

		void init(Physics* physics, ProjectileManager* projectileManager);
		void clear();
		void reset();
        

		void updateSpecific(float deltaTime);

		void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
		void onCollision(Projectile& other);

		void affect(int stacks, Effect const &effect, float deltaTime);
        void onEffectEnd(int stacks, Effect const &effect);
		void upgrade(Upgrade const &upgrade);

		void render(Graphics::Renderer& renderer); 
		void setMaxSpeed(float maxSpeed);

		void saveToFile();
		void readFromFile();

		void takeDamage(int damage, bool damageThroughProtection = false);
		int getHP() const;

		btKinematicCharacterController* getCharController();
		btGhostObject* getGhostObject();
		virtual DirectX::SimpleMath::Vector3 getPosition() const;

		void setPlayerState(PlayerState playerState);

		void setMoveSpeed(float speed);
		void setMoveDirection(btVector3 moveDir);

		btVector3 getForwardBT();
		btVector3 getMoveDirection();

		virtual btVector3 getPositionBT() const;
		virtual btTransform& getTransform() const;

		DirectX::SimpleMath::Vector3 getForward();
		DirectX::SimpleMath::Matrix getTransformMatrix() const;

		float getMoveSpeed() const;
		PlayerState getPlayerState() const;
		Sound::ListenerData& getListenerData();
        SkillManager* getSkillManager();

        const Weapon* getMainHand() const;
        const Weapon* getOffHand() const;
        const Skill* getSkill(int id) const;
        bool isUsingMeleeWeapon() const;
        int getCurrentWeapon() const;

		static btVector3 startPosition;

        void setCurrentSkills(int first, int second);
        int getCurrentSkill0() const;
        int getCurrentSkill1() const;

	};

}

#endif // !PLAYER_H
