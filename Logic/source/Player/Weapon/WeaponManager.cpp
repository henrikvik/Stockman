#include "Player/Weapon/WeaponManager.h"

using namespace Logic;

WeaponManager::WeaponManager() { }

WeaponManager::~WeaponManager() { }

void WeaponManager::init()
{
	initializeWeapons();
	makeWeaponLoadout();

	// Timers
	m_swapWeaponTimerMax = 1000.f;
	m_swapWeaponTimer = 0.f;
}

void WeaponManager::clear()
{
	m_weaponsLoadouts.clear();
	m_allWeapons.clear();
}

void Logic::WeaponManager::update(float deltaTime)
{
	// Timers / cooldowns
	if(m_swapWeaponTimer > 0.f)
		m_swapWeaponTimer -= deltaTime;
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
		printf("Switch weapon %d\n", index);
	}
}

void WeaponManager::usePrimary()
{
	m_currentWeapon.first->use();
	printf("Fire prim\n");
}

void WeaponManager::useSecondary()
{
	m_currentWeapon.second->use();
	printf("Fire sec\n");
}

bool Logic::WeaponManager::isSwitching()
{
	return m_swapWeaponTimer > 0.f;
}
