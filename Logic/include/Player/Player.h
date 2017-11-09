#ifndef PLAYER_H
#define PLAYER_H

#pragma region ClassDesc
		/*
			CLASS: Player
			AUTHOR:

			DESCRIPTION: Handles input-data from user
		*/
#pragma endregion

#include "Entity\Entity.h"

#include <Player\PlayerMovement.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

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
	private:
        PlayerMovement* m_pMovement;

		//ActionManager m_actionManager;
		WeaponManager* m_weaponManager;
		SkillManager* m_skillManager;
		Physics* m_physPtr;

		// UI States
		int m_hp;
        int currentWeapon;
        int currentSkills[2];

		// Sound
		Sound::ListenerData* m_listenerData;

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
		DirectX::Keyboard::Keys m_useSkillPrimary;
        DirectX::Keyboard::Keys m_useSkillSecondary;
        DirectX::Keyboard::Keys m_useSkillTertiary;

		// Sound
		void updateSound(float deltaTime);
	public:
		Player(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent);
		~Player();

		void init(Physics* physics, ProjectileManager* projectileManager);
		void clear();
		void reset();
        
		void updateSpecific(float deltaTime);

		void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);
		void onCollision(Projectile& other);

		void affect(int stacks, Effect const &effect, float deltaTime);
		void upgrade(Upgrade const &upgrade);

		void render() const; 

		void saveToFile();
		void readFromFile();

		void takeDamage(int damage, bool damageThroughProtection = false);
		int getHP() const;

		DirectX::SimpleMath::Matrix getTransformMatrix() const;
		Sound::ListenerData& getListenerData();
        SkillManager* getSkillManager();
        PlayerMovement* getPlayerMovement();

        const Weapon* getMainHand() const;
        const Weapon* getOffHand() const;
        const Skill* getSkill(int id) const;
        bool isUsingMeleeWeapon() const;
        int getCurrentWeapon() const;

        void setCurrentSkills(int first, int second);
        int getCurrentSkill0() const;
        int getCurrentSkill1() const;

	};

}

#endif // !PLAYER_H
