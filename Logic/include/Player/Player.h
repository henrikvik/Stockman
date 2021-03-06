#ifndef PLAYER_H
#define PLAYER_H

#pragma region ClassDesc
		/*
			CLASS: Player
			AUTHOR:

			DESCRIPTION: Handles input-data from user
		*/
#pragma endregion

#include <Graphics\include\RenderInfo.h>

#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "Entity\Entity.h"
#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

#define PLAYER_GRAVITY					9.82f * 2.f * 0.0000015f
#define PLAYER_SIZE_RADIUS				0.5f
#define PLAYER_SIZE_HEIGHT				2.f
#define PLAYER_EYE_OFFSET               {0.f, PLAYER_SIZE_HEIGHT * 0.5f, 0.f}
#define PLAYER_STARTING_HP				3
#define PLAYER_MOUSE_SENSETIVITY		0.1f
#define PLAYER_MOVEMENT_MAX_SPEED		0.015f

namespace Sound
{
    struct ListenerData;
}

namespace Logic
{
    class Physics;
    class WeaponManager;
    class Weapon;
    class AmmoContainer;
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
        static const int MIN_Y, MAX_HP;

        // Special modes (move to other class)
        bool m_godMode, m_noclip;

		btKinematicCharacterController* m_charController;

		//ActionManager m_actionManager;
		WeaponManager* m_weaponManager;
		SkillManager* m_skillManager;
		Physics* m_physPtr;

        // AI
        Entity *m_targetedBy; // entity that "targets" player

		// UI States
		int m_hp;
        int currentWeapon;
        int currentSkills[2];

		// Movements
        btVector3 m_lastPos;
		PlayerState m_playerState;
		DirectX::SimpleMath::Vector3 m_forward;
		float m_moveMaxSpeed;
		btVector3 m_moveDir; // only 2 dimensional movement direction (x, z)
		float m_moveSpeed;
        float m_moveSpeedMod;
        float m_permanentSpeedMod;
        float m_jumpSpeedMod;
		float m_acceleration;
		float m_deacceleration;
		float m_airAcceleration;
		float m_jumpSpeed;

		bool m_wishJump;
        bool m_firstJump;
		btVector3 m_wishDir;
		float m_wishDirForward;
		float m_wishDirRight;

        bool m_wasInAir;

        float m_damageTintTimer;
        float m_upgradeTintTimer;
        float m_pickupTintTimer;

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

        DirectX::SimpleMath::Vector2 getWindowMidPoint();

        // Player step
        void stepPlayer(float deltaTime);

        void updateScreenTint(float deltaTime);

		// Sound
		void updateSound(float deltaTime);

        //status
        bool m_stunned;
	public:
		Player(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent);
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

        virtual void onUpgradeAdd(int stacks, Upgrade const &upgrade);

		void render() const; 
		void setMaxSpeed(float maxSpeed);

		void saveToFile();
		void readFromFile();

		void takeDamage(int damage, bool damageThroughProtection = false);
        int getHP() const;
        int getMaxHP() const;

		btKinematicCharacterController* getCharController();
		btGhostObject* getGhostObject();

		void setPlayerState(PlayerState playerState);

		void setMoveSpeed(float speed);
		void setMoveDirection(btVector3 moveDir);

		btVector3 getForwardBT();
		btVector3 getMoveDirection();

        virtual DirectX::SimpleMath::Vector3 getPosition() const;
        virtual DirectX::SimpleMath::Vector3 getEyePosition() const;
		virtual btVector3 getPositionBT() const;
		virtual btTransform& getTransform() const;

        float getYaw() const;
        float getPitch() const;
		DirectX::SimpleMath::Vector3 getForward() const;
		DirectX::SimpleMath::Matrix getTransformMatrix() const;
        DirectX::SimpleMath::Matrix getEyeTransformMatrix() const;

		float getMoveSpeed() const;
		PlayerState getPlayerState() const;
		Sound::ListenerData& getListenerData();
        SkillManager* getSkillManager();

        const AmmoContainer& getActiveAmmoContainer() const;
        const AmmoContainer& getInactiveAmmoContainer() const;
        const Skill* getSkill(int id) const;
        bool isUsingMeleeWeapon() const;
        int getCurrentWeapon() const;

		static btVector3 startPosition;

        void setCurrentSkills(int first, int second);
        int getCurrentSkill0() const;
        int getCurrentSkill1() const;
        bool getReloding() const;
        int getAmmoPickedUp();
        void setPickupTintTimer(float time);

        // AI
        void setTargetedBy(Entity *entity);
        bool isTargeted();
        void resetTargeted();
	};

}

#endif // !PLAYER_H
