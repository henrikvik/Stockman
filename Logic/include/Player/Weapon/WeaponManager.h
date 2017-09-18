#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H
#pragma region ClassDesc
		/*
			CLASS: WeaponManager
			AUTHOR:

			DESCRIPTION: This class is made to manage the weapons of the system.
		*/
#pragma endregion

#include <vector>
#include <Player\Weapon\Weapon.h>
#include <Misc\Enums.h>
#include <Projectile\ProjectileStruct.h>
#include <Graphics\include\Structs.h>


namespace Logic
{
	class WeaponManager
	{

	public:		
		WeaponManager();
		WeaponManager(const WeaponManager& other) = delete;
		WeaponManager* operator=(const WeaponManager& other) = delete;
		~WeaponManager();

		void init();
		void clear();
		void update(float deltaTime);

		void switchWeapon(int weaponID);
		void usePrimary();
		void useSecondary();
		void reloadWeapon();

		bool isSwitching();
		bool isAttacking();
		bool isReloading();

		std::vector<ProjectileData*>* getProjectileList();
		void clearProjectileList();

	private:

		void initializeWeapons();
		void makeWeaponLoadout();

		std::vector<Weapon> m_allWeapons;
		std::vector<std::pair<Weapon*, Weapon*>> m_weaponsLoadouts;
		//std::vector<int> ammoList;
		std::pair<Weapon*, Weapon*> m_currentWeapon;
		std::vector<ProjectileData*> m_projToFire;						// List of all player projectiles to be created this frame

		// Timers
		float m_swapWeaponTimer;
		float m_swapWeaponTimerMax;
		float m_attackTimer;

		float m_reloadTimer;
		ReloadingWeapon m_reloadState;
	};
}
#endif
