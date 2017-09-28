#include "Player/Weapon/WeaponManager.h"

using namespace Logic;

WeaponManager::WeaponManager() { }

WeaponManager::~WeaponManager() { clear(); }

void WeaponManager::init(ProjectileManager* projectileManager)
{
	m_projectileManager = projectileManager;
	initializeWeapons();
	makeWeaponLoadout();
	m_currentWeapon = m_weaponsLoadouts[0];

	// Timers
	m_swapWeaponTimerMax = 1000.f;
	m_swapWeaponTimer = 0.f;
	m_attackTimer = 0.f;

	m_reloadTimer = 0.f;
	m_reloadState = ReloadingWeapon::IDLE;
}

void WeaponManager::clear()
{
	m_weaponsLoadouts.clear();
	m_allWeapons.clear();
}

void WeaponManager::setWeaponModel(DirectX::SimpleMath::Matrix playerTranslation, DirectX::SimpleMath::Vector3 playerForward)
{
	// Updating weapon model
	m_currentWeapon.first->setWeaponModelFrontOfPlayer(playerTranslation, playerForward);
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
		m_currentWeapon.first->fillMag();
		m_reloadState = ReloadingWeapon::IDLE;
		printf("adding ammo\n");
		printf("ammo: %d\n", m_currentWeapon.first->getAmmo());
		printf("mag: %d\n", m_currentWeapon.first->getMagAmmo());
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
	m_currentWeapon.first->render(renderer);
}

void WeaponManager::initializeWeapons()
{
	// Adding all weapons
	m_allWeapons =
	{
		{ m_projectileManager, ProjectileData(1, 0.2, 1, 100, 0, 3000, Graphics::ModelID::CUBE, 1), 0, 60, 60, 30, 30, 1, 1, 0, 0, 450, 1, 2000 },		// Gattling prim
		{ m_projectileManager, ProjectileData(1, 0.1f, 1, 100, 0, 500, Graphics::ModelID::CUBE, 1), 1, 60, 60, 8, 8, 6, 18, 15, 10, 100, 1, 2000 },		// Gattling sec
		{ m_projectileManager, ProjectileData(1, 1, 1, 10, 0, 500, Graphics::ModelID::CUBE, 1), 2, 100, 100, 50, 50, 1, 1, 0, 0, 750, 1, 3000 },		// Freeze prim
		{ m_projectileManager, ProjectileData(1, 1, 1, 80, 8, 5000, Graphics::ModelID::CUBE, 1), 3, 100, 100, 50, 50, 10, 1, 0, 0, 100, 1, 3000 },		// Freeze sec
		{ m_projectileManager, ProjectileData(1, 2, 1, 0, 0, 100, Graphics::ModelID::CUBE, 1), 4, 0, 0, 0, 0, 0, 1, 0, 0, 50, 1, 3000 },				// Sledge prim
		{ m_projectileManager, ProjectileData(1, 2, 1, 0, 0, 100, Graphics::ModelID::CUBE, 1), 5, 0, 0, 0, 0, 0, 1, 0, 0, 50, 1, 3000 }					// Sledge sec
	};
}

void WeaponManager::makeWeaponLoadout()
{
	// Adding current weapon pair
	m_weaponsLoadouts =
	{ 
		{ &m_allWeapons[0], &m_allWeapons[1] },
		{ &m_allWeapons[2], &m_allWeapons[3] },
		{ &m_allWeapons[4], &m_allWeapons[5] }
	};
}

void WeaponManager::switchWeapon(int index)
{
	// Check if swapTimer is active or swapping to same weapon 
	if (m_swapWeaponTimer <= 0.f && !(m_currentWeapon == m_weaponsLoadouts[index]))
	{
		m_currentWeapon = m_weaponsLoadouts[index];
		m_swapWeaponTimer = m_swapWeaponTimerMax;

		m_reloadTimer = 0.f;
		m_reloadState = ReloadingWeapon::IDLE;

		printf("switch weapon %d\n", index);
	}
}

void WeaponManager::usePrimary(btVector3 position, float yaw, float pitch)
{
	if(m_attackTimer <= 0.f)
	{
		if (m_currentWeapon.first->getMagAmmo() > 0 || m_currentWeapon.first->getAmmoConsumption() == 0)
		{
			m_currentWeapon.first->use(position, yaw, pitch);
			m_currentWeapon.first->removeMagAmmo(m_currentWeapon.first->getAmmoConsumption());
			printf("fire prim\n");
			printf("mag: %d\n", m_currentWeapon.first->getMagAmmo());
		}
		else
			printf("out of ammo\n");

		m_attackTimer = m_currentWeapon.first->getAttackTimer();
	}
}

void WeaponManager::useSecondary(btVector3 position, float yaw, float pitch)
{
	if (m_attackTimer <= 0.f)
	{
		if (m_currentWeapon.first->getMagAmmo() > 0 || m_currentWeapon.second->getAmmoConsumption() == 0)
		{
			m_currentWeapon.second->use(position, yaw, pitch);
			m_currentWeapon.first->removeMagAmmo(m_currentWeapon.second->getAmmoConsumption());
			printf("fire sec\n");
			printf("mag: %d\n", m_currentWeapon.first->getMagAmmo());
		}
		else
			printf("out of ammo\n");

		m_attackTimer = m_currentWeapon.second->getAttackTimer();
	}
}

void Logic::WeaponManager::reloadWeapon()
{
	if (m_reloadTimer <= 0.f && m_currentWeapon.first->getAmmo() > 0 && m_currentWeapon.first->getMagAmmo() < m_currentWeapon.first->getMagSize())
	{
		m_reloadTimer = m_currentWeapon.first->getRealoadTime();
		m_reloadState = ReloadingWeapon::ACTIVE;
		printf("reloading weapon\n");
	}
}

bool Logic::WeaponManager::isSwitching()
{
	return m_swapWeaponTimer > 0.f;
}

bool Logic::WeaponManager::isAttacking()
{
	return m_attackTimer > 0.f;
}

bool Logic::WeaponManager::isReloading()
{
	return m_reloadState != ReloadingWeapon::IDLE;
}
