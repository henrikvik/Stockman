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


namespace Graphics
{
    class Renderer;
}

namespace Logic
{
    class Entity;
    class Player;
    class Weapon;
    class AmmoContainer;
    class WeaponModel;
    class ProjectileManager;
    class Effect;

	class WeaponManager
	{
	public:
        struct WeaponLoadout
        {
            Weapon* primary;
            Weapon* secondary;
            AmmoContainer* ammoContainer;
            WeaponModel* weaponModel;

            bool operator==(const WeaponLoadout &other)
            {
                return (primary == other.primary && secondary == other.secondary && ammoContainer == other.ammoContainer && weaponModel && other.weaponModel);
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
        void affect(Effect const & effect);
		void render() const;

		void setWeaponModel(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward);

		void switchWeapon(int weaponID);

        void tryUsePrimary(btVector3 position, float yaw, float pitch, Player& shooter);
		void usePrimary(btVector3 position, float yaw, float pitch, Entity& shooter);
        void tryUseSecondary(btVector3 position, float yaw, float pitch, Player& shooter);
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

        enum WeaponToUse
        {
            USE_NOTHING,
            USE_PRIMARY,
            USE_SECONDARY
        };

        struct Upgrades
        {
            int magSizeModifier;
            int ammoCapModifier;
            float fireRateModifier;
            float freezeDurationModifier;
            int fireDamageModifier;
        };
		void initializeWeapons(ProjectileManager* projectileManager);

        Upgrades m_Upgrades;

        std::vector<WeaponLoadout*> m_weaponLoadouts;
		WeaponLoadout* m_currentWeapon;

		// Timers
		float m_attackRateTimer;
        WeaponToUse m_toUse;
        Player* m_toUseShooter;

		float m_reloadTimer;
		ReloadingWeapon m_reloadState;
	};
}
#endif
