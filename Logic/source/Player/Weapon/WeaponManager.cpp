#include "Player/Weapon/WeaponManager.h"
#include <Player\Weapon\Weapon.h>
#include <Graphics\include\Structs.h>
#include <Projectile\ProjectileStruct.h>
#include <Player\Weapon\AmmoContainer.h>

using namespace Logic;

WeaponManager::WeaponManager() { }

WeaponManager::~WeaponManager() { clear(); }

void WeaponManager::init(ProjectileManager* projectileManager)
{
	initializeWeapons(projectileManager);
	m_currentWeapon = m_weaponLoadouts[0];

	// Timers
	m_swapWeaponTimerMax = 1000.f;
	m_swapWeaponTimer = 0.f;
	m_attackTimer = 0.f;

	m_reloadTimer = 0.f;
	m_reloadState = ReloadingWeapon::IDLE;
}

void WeaponManager::clear()
{
    for (WeaponLoadout* weaponLoadout : m_weaponLoadouts)
    {
        delete weaponLoadout->primary;
        delete weaponLoadout->secondary;
        delete weaponLoadout->ammoContainer;
        delete weaponLoadout;
    }
        
	m_weaponLoadouts.clear();
}

void WeaponManager::reset()
{
	for (int i = 0; i < m_weaponLoadouts.size(); i++)
        m_weaponLoadouts[i]->ammoContainer->reset();
}

void WeaponManager::setWeaponModel(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
	// Updating weapon model
	m_currentWeapon->primary->setWeaponModelFrontOfPlayer(playerTranslation, playerForward);
}

void WeaponManager::update(float deltaTime)
{
	// Timers / cooldowns

	// Reload
	if (m_reloadTimer > 0.f)
	{
		m_reloadTimer -= deltaTime;
	}
	else if (m_reloadState == ReloadingWeapon::ACTIVE)
	{
		m_reloadState = ReloadingWeapon::DONE;
	}
	if (m_reloadState == ReloadingWeapon::DONE)
	{
		m_currentWeapon->ammoContainer->fillMag();
		m_reloadState = ReloadingWeapon::IDLE;
		printf("adding ammo\n");
		printf("ammo: %d\n", m_currentWeapon->ammoContainer->getAmmoInfo().ammo);
		printf("mag: %d\n", m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo);
	}

	// Attack
	if (m_attackTimer > 0.f)
		m_attackTimer -= deltaTime;
	// Weapon swap
	if (m_swapWeaponTimer > 0.f)
	{
		m_swapWeaponTimer -= deltaTime;
		m_attackTimer = m_swapWeaponTimer;
	}
}

void WeaponManager::render(Graphics::Renderer& renderer)
{
	m_currentWeapon->primary->render(renderer);
}

void WeaponManager::initializeWeapons(ProjectileManager* projectileManager)
{
    // Adding current weapon pairs
    m_weaponLoadouts =
    {
        // Gattling
        new WeaponLoadout{ 
        /* Primary */   new Weapon(Graphics::ModelID::CROSSBOW, projectileManager, ProjectileData(1, 0.2, 1, 100, 0.f, 3000, Graphics::ModelID::CUTTLERY, 1), Weapon::WeaponInfo{ 0, 1, 0, 0, 450, 0 }),
        /* Secondary*/  new Weapon(Graphics::ModelID::CROSSBOW, projectileManager, ProjectileData(1, 0.1f, 1, 100, 0.f, 500, Graphics::ModelID::CUTTLERY, 1), Weapon::WeaponInfo{ 1, 18, 15, 10, 100, 0 }),
        /* AmmoContainer */ new AmmoContainer(AmmoContainer::AmmoInfo{ 60, 60, 30, 30, 1, 6, 2000 }) },

        /* Primary */new WeaponLoadout{ new Weapon(Graphics::ModelID::STAFF, projectileManager, ProjectileData(1, 1, 1, 20, 0, 675, Graphics::ModelID::CUBE, 1, ProjectileType::ProjectileTypeIce, true), Weapon::WeaponInfo{ 2, 1, 17, 5, 750, 1 }),
        /* Secondary*/new Weapon(Graphics::ModelID::STAFF, projectileManager, ProjectileData(1, 1, 1, 50, 5, 5000, Graphics::ModelID::CUBE, 1), Weapon::WeaponInfo{ 3, 1, 0, 0, 100, 1 }),
        /* AmmoContainer */ new AmmoContainer(AmmoContainer::AmmoInfo{ 300, 200, 100, 100, 1, 10, 3000 }) },

        /* Primary */new WeaponLoadout{ new Weapon(Graphics::ModelID::STAFF, projectileManager, ProjectileData(1, 2, 1, 0, 0, 300, Graphics::ModelID::CUBE, 1, ProjectileType::ProjectileTypeMelee, true), Weapon::WeaponInfo{ 4, 1, 0, 0, 50, 0 }),
        /* Secondary*/new Weapon(Graphics::ModelID::STAFF, projectileManager, ProjectileData(1, 2, 1, 0, 0, 300, Graphics::ModelID::CUBE, 1, ProjectileType::ProjectileTypeMelee, true), Weapon::WeaponInfo{ 5, 1, 0, 0, 50, 0 }),
        /* AmmoContainer */ new AmmoContainer(AmmoContainer::AmmoInfo{ 0, 0, 0, 0, 0, 0, 0 }) }
    };
}

void WeaponManager::switchWeapon(int index)
{
	// Check if swapTimer is active or swapping to same weapon 
	if (m_swapWeaponTimer <= 0.f && !(m_currentWeapon == m_weaponLoadouts[index]))
	{
		m_currentWeapon = m_weaponLoadouts[index];
		m_swapWeaponTimer = m_swapWeaponTimerMax;

		m_reloadTimer = 0.f;
		m_reloadState = ReloadingWeapon::IDLE;

		printf("switch weapon %d\n", index);
	}
}

void WeaponManager::usePrimary(btVector3 position, float yaw, float pitch, Entity& shooter)
{
	if(m_attackTimer <= 0.f)
	{
		if (m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo > 0 || m_currentWeapon->ammoContainer->getAmmoInfo().primAmmoConsumption == 0)
		{
			m_currentWeapon->primary->use(position, yaw, pitch, shooter);
			m_currentWeapon->ammoContainer->removePrimaryAmmo();
		}
		else
			printf("out of ammo\n");

		m_attackTimer = m_currentWeapon->primary->getAttackTimer();
	}
}

void WeaponManager::useSecondary(btVector3 position, float yaw, float pitch, Entity& shooter)
{
	if (m_attackTimer <= 0.f)
	{
		if (m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo > 0 || m_currentWeapon->ammoContainer->getAmmoInfo().secAmmoConsumption == 0)
		{
			m_currentWeapon->secondary->use(position, yaw, pitch, shooter);
            m_currentWeapon->ammoContainer->removeSecondaryAmmo();
		}
		else
			printf("out of ammo\n");

		m_attackTimer = m_currentWeapon->secondary->getAttackTimer();
	}
}

void WeaponManager::reloadWeapon()
{
	if (m_reloadTimer <= 0.f && m_currentWeapon->ammoContainer->getAmmoInfo().ammo > 0 && m_currentWeapon->ammoContainer->getAmmoInfo().magAmmo < m_currentWeapon->ammoContainer->getAmmoInfo().magSize)
	{
        m_reloadTimer = m_currentWeapon->ammoContainer->getAmmoInfo().reloadTime;
		m_reloadState = ReloadingWeapon::ACTIVE;
		printf("reloading weapon\n");
	}
}

bool WeaponManager::isSwitching()
{
	return m_swapWeaponTimer > 0.f;
}

bool WeaponManager::isAttacking()
{
	return m_attackTimer > 0.f;
}

bool WeaponManager::isReloading()
{
	return m_reloadState != ReloadingWeapon::IDLE;
}

WeaponManager::WeaponLoadout* WeaponManager::getCurrentWeaponLoadout()
{
	return m_currentWeapon;
}

WeaponManager::WeaponLoadout* WeaponManager::getWeaponLoadout(int index)
{
    return m_weaponLoadouts[index];
}

WeaponManager::WeaponLoadout* WeaponManager::getActiveWeaponLoadout()
{
    if (m_weaponLoadouts[0] == m_currentWeapon)
    {
        return m_weaponLoadouts[0];
    }
    else
    {
        return m_weaponLoadouts[1];
    } 
}

WeaponManager::WeaponLoadout* WeaponManager::getInactiveWeaponLoadout()
{
    if (m_weaponLoadouts[1] == m_currentWeapon)
    {
        return m_weaponLoadouts[0];
    }
    else
    {
        return m_weaponLoadouts[1];
    };
}
