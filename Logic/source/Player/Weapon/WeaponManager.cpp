#include "Player/Weapon/WeaponManager.h"

using namespace Logic;

WeaponManager::WeaponManager() { }

WeaponManager::~WeaponManager() { }

void WeaponManager::init()
{
	initializeWeapons();
	makeWeaponLoadout();
}

void WeaponManager::clear()
{
	m_weaponsLoadouts.clear();
	m_allWeapons.clear();
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
	m_currentWeapon = m_weaponsLoadouts[index];
}

void WeaponManager::usePrimary()
{
	m_currentWeapon.first->use();
}

void WeaponManager::useSecondary()
{
	m_currentWeapon.second->use();
}
