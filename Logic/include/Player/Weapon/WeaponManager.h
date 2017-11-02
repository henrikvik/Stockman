#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H
#pragma region ClassDesc
		/*
			CLASS: WeaponManager
			AUTHOR:

			DESCRIPTION: This class is made to manage the weapons of the system.
		*/
#pragma endregion

#include <d3d11.h>
#include <SimpleMath.h>

#include <vector>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Graphics
{
    class Renderer;
}

namespace Logic
{
    class Entity;
    class Weapon;
    class ProjectileManager;

	class WeaponManager
	{
	public:		
		WeaponManager();
		WeaponManager(const WeaponManager& other) = delete;
		WeaponManager* operator=(const WeaponManager& other) = delete;
		~WeaponManager();

		void init(ProjectileManager* projectileManager);
		void clear();
		void reset();
		void update(float deltaTime);
		void render() const;

		void setWeaponModel(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward);

		void switchWeapon(int weaponID);
		void usePrimary(btVector3 position, float yaw, float pitch, Entity& shooter);
		void useSecondary(btVector3 position, float yaw, float pitch, Entity& shooter);
		void reloadWeapon();

		bool isSwitching();
		bool isAttacking();
		bool isReloading();

		Weapon* getCurrentWeaponPrimary();
		Weapon* getCurrentWeaponSecondary();

        std::pair<Weapon*, Weapon*> getFirstWeapon();
        std::pair<Weapon*, Weapon*> getSecondWeapon();
        Weapon* getActiveWeapon();
        Weapon* getInactiveWeapon();

	private:

		enum ReloadingWeapon
		{
			IDLE,
			ACTIVE,
			DONE
		};

		void initializeWeapons();
		void makeWeaponLoadout();

		ProjectileManager* m_projectileManager;
		std::vector<Weapon*> m_allWeapons;
		std::vector<std::pair<Weapon*, Weapon*>> m_weaponsLoadouts;
		//std::vector<int> ammoList;
		std::pair<Weapon*, Weapon*> m_currentWeapon;

		// Timers
		float m_swapWeaponTimer;
		float m_swapWeaponTimerMax;
		float m_attackTimer;

		float m_reloadTimer;
		ReloadingWeapon m_reloadState;
	};
}
#endif
