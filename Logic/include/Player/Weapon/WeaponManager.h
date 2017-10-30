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
#include <d3d11.h>
#include <SimpleMath.h>
#include <btBulletCollisionCommon.h>

namespace Graphics
{
    class Renderer;
}

namespace Logic
{
    class Entity;
    class Weapon;
    class AmmoContainer;
    class ProjectileManager;

	class WeaponManager
	{
	public:
        struct WeaponLoadout
        {
            Weapon* primary;
            Weapon* secondary;
            AmmoContainer* ammoContainer;

            bool operator==(const WeaponLoadout &other)
            {
                return (primary == other.primary && secondary == other.secondary && ammoContainer == other.ammoContainer);
            }
        };

		WeaponManager();
		WeaponManager(const WeaponManager& other) = delete;
		WeaponManager* operator=(const WeaponManager& other) = delete;
		~WeaponManager();

		void init(ProjectileManager* projectileManager);
		void clear();
		void reset();
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

        WeaponLoadout* getCurrentWeaponLoadout();
        WeaponLoadout* getWeaponLoadout(int index);
        WeaponLoadout* getActiveWeaponLoadout();
        WeaponLoadout* getInactiveWeaponLoadout();

	private:
        enum ReloadingWeapon
        {
            IDLE,
            ACTIVE,
            DONE
        };

		void initializeWeapons(ProjectileManager* projectileManager);

        std::vector<WeaponLoadout*> m_weaponLoadouts;
		WeaponLoadout* m_currentWeapon;

		// Timers
		float m_swapWeaponTimer;
		float m_swapWeaponTimerMax;
		float m_attackTimer;

		float m_reloadTimer;
		ReloadingWeapon m_reloadState;
	};
}
#endif
