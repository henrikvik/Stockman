#include "Player/Weapon/WeaponManager.h"

using namespace Logic;

WeaponManager::WeaponManager() { }

WeaponManager::~WeaponManager() { }

void WeaponManager::init()
{
	initializeWeapons();
	makeWeaponLoadout();
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
		{ &std::make_pair<Weapon*, Weapon*>(&m_allWeapons[0], &m_allWeapons[1]) },
		{ &std::make_pair<Weapon*, Weapon*>(&m_allWeapons[2], &m_allWeapons[3]) },
		{ &std::make_pair<Weapon*, Weapon*>(&m_allWeapons[4], &m_allWeapons[5]) }
	};
}

void WeaponManager::switchWeapon(int index)
{
	m_currentWeapon = m_weaponsLoadouts[index];
}

void WeaponManager::switchToPrimary()
{
	m_currentWeaponMode = m_currentWeapon.first;
}

void WeaponManager::switchToSecondary()
{
	m_currentWeaponMode = m_currentWeapon.second;
}

void WeaponManager::useCurrentWeapon()
{
	m_currentWeaponMode->use();
}