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

#include <Player\Weapon\AmmoContainer.h>
#include <Player\Weapon\ModelAnimation.h>

#define WEAPON_PRIMARY 0
#define WEAPON_SECONDARY 1

namespace Graphics
{
    class Renderer;
}

namespace Logic
{
    class Entity;
    class Player;
    class Weapon;
    class ProjectileManager;
    class Effect;
    class Upgrade;

	class WeaponManager
	{
	public:
        struct WeaponLoadout
        {
            Weapon* weapon[2];
            AmmoContainer ammoContainer;
            WeaponAnimation* weaponModel;
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
        void onUpgradeAdd(int stacks, Upgrade const & upgrade);
		void render() const;

		void setWeaponModel(float deltaTime, DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward);

		void switchWeapon(int weaponID);

        void tryAttack(int attackMode, btVector3 position, float yaw, float pitch, Player& shooter);
		void attack(int attackMode, btVector3 position, float yaw, float pitch, Entity& shooter);
		void reloadWeapon();

		bool isSwitching();
		bool isAttacking();
		bool isReloading();

        WeaponLoadout* getCurrentWeaponLoadout();
        WeaponLoadout* getWeaponLoadout(int index);
        WeaponLoadout* getActiveWeaponLoadout();
        WeaponLoadout* getInactiveWeaponLoadout();

        int getAmmoPickedUp();

	private:
        enum ReloadingWeapon
        {
            IDLE,
            ACTIVE,
            DONE
        };

        enum WeaponToUse
        {
            USE_PRIMARY,
            USE_SECONDARY, 
            USE_NOTHING
        };

        struct Upgrades
        {
            float magSizeModifier;
            int ammoCapModifier;
            float fireRateModifier;
            float reloadTimeModifier;
            float freezeDurationModifier;
            int fireDamageModifier;
        };
		void initializeWeapons(ProjectileManager* projectileManager);

        Upgrades m_Upgrades;

        std::vector<WeaponLoadout> m_weaponLoadouts;
		WeaponLoadout* m_currentWeapon;

		// Timers
		float m_attackRateTimer;
        WeaponToUse m_toUse;
        Player* m_toUseShooter;

		float m_reloadTimer;
		ReloadingWeapon m_reloadState;

        int m_ammoPickedUp;
	};
}
#endif
