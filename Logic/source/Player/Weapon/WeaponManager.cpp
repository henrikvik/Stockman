#include "Player/Weapon/WeaponManager.h"

using namespace Logic;

WeaponManager::WeaponManager() { }

WeaponManager::~WeaponManager() { clear(); }

void WeaponManager::init()
{
	initializeWeapons();
	makeWeaponLoadout();

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
		// ADD AMMO
		m_reloadState = ReloadingWeapon::reloadingWeaponIdle;
		printf("adding ammo\n");
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
		{ 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1 }
	};
}

void WeaponManager::makeWeaponLoadout()
{
	// Adding current weapon pair
	m_weaponsLoadouts =
	{ 
		{ std::make_pair<Weapon*, Weapon*>(&m_allWeapons[0], &m_allWeapons[1]) },
		{ std::make_pair<Weapon*, Weapon*>(&m_allWeapons[2], &m_allWeapons[3]) },
		{ std::make_pair<Weapon*, Weapon*>(&m_allWeapons[4], &m_allWeapons[5]) }
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

void WeaponManager::usePrimary()
{
	if(m_attackTimer <= 0.f)
	{
		m_currentWeapon.first->use();
		//m_attackTimer = m_currentWeapon.first->firerate
		m_attackTimer = 1000.f;
		printf("fire prim\n");
	}
}

void WeaponManager::useSecondary()
{
	if (m_attackTimer <= 0.f)
	{
		m_currentWeapon.second->use();
		//m_attackTimer = m_currentWeapon.second->firerate
		m_attackTimer = 1000.f;
		printf("fire sec\n");
	}
}

void Logic::WeaponManager::reloadWeapon()
{
	if (m_reloadTimer <= 0.f /* && !!CHECK AMMO!! */)
	{
		// Reload weapon
		m_reloadTimer = 3000.f; // Replace with weapon reload
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
