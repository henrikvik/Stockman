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
	m_reloadState = ReloadingWeapon::reloadingWeaponIdle;
}

void WeaponManager::clear()
{
	m_weaponsLoadouts.clear();
	m_allWeapons.clear();
}

void Logic::WeaponManager::update(float deltaTime)
{
	// Timers / cooldowns

	// Reload
	if (m_reloadTimer > 0.f)
	{
		m_reloadTimer -= deltaTime;
	}
	else if (m_reloadState == ReloadingWeapon::reloadingWeaponActive)
	{
		m_reloadState = ReloadingWeapon::reloadingWeaponDone;
	}
	if (m_reloadState == ReloadingWeapon::reloadingWeaponDone)
	{
		m_currentWeapon.first->fillMag();
		m_reloadState = ReloadingWeapon::reloadingWeaponIdle;
		printf("adding ammo\n");
		printf("ammo: %d\n", m_currentWeapon.first->getAmmo());
		printf("mag: %d\n", m_currentWeapon.first->getMagAmmo());
	}

	// Attack
	if (m_attackTimer > 0.f)
	{
		m_attackTimer -= deltaTime;
	}
	// Weapon swap
	if (m_swapWeaponTimer > 0.f)
	{
		m_swapWeaponTimer -= deltaTime;
		m_attackTimer = m_swapWeaponTimer;
	}
}

void WeaponManager::initializeWeapons()
{
	// Adding all weapons
	m_allWeapons =
	{
		{ m_projectileManager, ProjectileData(1, 1, 1, 100, 1, Graphics::ModelID::CUBE, 1), 1, 60, 60, 30, 30, 1, 1, 450, 1, 3000 },
		{ m_projectileManager, ProjectileData(1, 1, 1, 10, 1, Graphics::ModelID::CUBE, 1), 1, 20, 20, 8, 8, 2, 1, 40, 1, 3000 },
		{ m_projectileManager, ProjectileData(1, 1, 1, 10, 1, Graphics::ModelID::CUBE, 1), 1, 20, 20, 8, 8, 1, 1, 1, 1, 3000 },
		{ m_projectileManager, ProjectileData(1, 1, 1, 10, 1, Graphics::ModelID::CUBE, 1), 1, 20, 20, 8, 8, 3, 1, 1, 1, 3000 },
		{ m_projectileManager, ProjectileData(1, 1, 1, 10, 1, Graphics::ModelID::CUBE, 1), 1, 20, 20, 8, 8, 0, 1, 1, 1, 3000 },
		{ m_projectileManager, ProjectileData(1, 1, 1, 10, 1, Graphics::ModelID::CUBE, 1), 1, 20, 20, 8, 8, 0, 1, 1, 1, 3000 }
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
		m_reloadState = ReloadingWeapon::reloadingWeaponIdle;

		printf("switch weapon %d\n", index);
	}
}

void WeaponManager::usePrimary(btVector3 position, btVector3 forward)
{
	if(m_attackTimer <= 0.f)
	{
		if (m_currentWeapon.first->getMagAmmo() > 0)
		{
			m_currentWeapon.first->use(position, forward);
			m_currentWeapon.first->removeMagAmmo(m_currentWeapon.first->getAmmoConsumption());
			printf("fire prim\n");
			printf("mag: %d\n", m_currentWeapon.first->getMagAmmo());
		}
		else
			printf("out of ammo\n");

		m_attackTimer = m_currentWeapon.first->getAttackTimer();
		//m_attackTimer = 1000.f;
	}
}

void WeaponManager::useSecondary(btVector3 position, btVector3 forward)
{
	if (m_attackTimer <= 0.f)
	{
		if (m_currentWeapon.first->getMagAmmo() > 0)
		{
			m_currentWeapon.second->use(position, forward);
			m_currentWeapon.first->removeMagAmmo(m_currentWeapon.second->getAmmoConsumption());
			printf("fire sec\n");
			printf("mag: %d\n", m_currentWeapon.first->getMagAmmo());
		}
		else
			printf("out of ammo\n");

		m_attackTimer = m_currentWeapon.second->getAttackTimer();
		//m_attackTimer = 1000.f;
	}
}

void Logic::WeaponManager::reloadWeapon()
{
	if (m_reloadTimer <= 0.f && m_currentWeapon.first->getAmmo() > 0 && m_currentWeapon.first->getMagAmmo() < m_currentWeapon.first->getMagSize())
	{
		m_reloadTimer = 3000.f; // Replace with weapon reload speed
		m_reloadState = ReloadingWeapon::reloadingWeaponActive;
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
	return m_reloadState != ReloadingWeapon::reloadingWeaponIdle;
}
