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
#include <Projectile\ProjectileManager.h>
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

		void init(ProjectileManager* projectileManager);
		void clear();
		void update(float deltaTime);
		void render(Graphics::Renderer& renderer);

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

        Weapon* getfirstWeapon();
        Weapon* getSecondWeapon();

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
		std::vector<Weapon> m_allWeapons;
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
