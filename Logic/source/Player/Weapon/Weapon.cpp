#include "Player/Weapon/Weapon.h"

using namespace Logic;

Weapon::Weapon()
{
	m_weaponID			= -1;
	m_ammoCap			= -1;
	m_ammo				= -1;
	m_magSize			= -1;
	m_magAmmo			= -1;
	m_ammoConsumption	= -1;
	m_damage			= -1;
	m_attackRate		= -1;
	m_freeze			= -1;
	m_reloadTime		= -1;
}

Weapon::Weapon(ProjectileManager* projectileManager, ProjectileData projectileData, int weaponID, int ammoCap, int ammo, int magSize, int magAmmo, int ammoConsumption, int projectileCount,
	float spreadH, float spreadV, float damage, float attackRate, float freeze, float reloadTime)
{
	m_projectileManager = projectileManager;
	m_weaponID			= weaponID;
	m_ammoCap			= ammoCap;
	m_ammo				= ammo;
	m_magSize			= magSize;
	m_magAmmo			= magAmmo;
	m_ammoConsumption	= ammoConsumption;
	m_projectileCount	= projectileCount;
	m_spreadH			= spreadH;
	m_spreadV			= spreadV;
	m_damage			= damage;
	m_attackRate		= attackRate;
	m_freeze			= freeze;
	m_reloadTime		= reloadTime;
	m_projectileData	= projectileData;
}

void Weapon::use(btVector3 position, btVector3 forward)
{
	// use weapon

	for (int i = m_projectileCount; i--; )
	{
		if (m_spreadH != 0 || m_spreadV !=0)
		{
			btVector3 nForward = calcSpread(forward);
			m_projectileManager->addProjectile(m_projectileData, position, nForward);
		}
		else
		{
			m_projectileManager->addProjectile(m_projectileData, position, forward);
		}
	}
}

btVector3 Logic::Weapon::calcSpread(btVector3 forward)
{
	if (m_spreadH != 0)
	{
		int sMagnified = m_spreadH * 1000;
		float rs = ((rand() % (2 * sMagnified)) - sMagnified) * 0.001f;
		forward = forward.rotate(btVector3(0, 1, 0), rs);
	}

	if (m_spreadV != 0)
	{
		int sMagnified = m_spreadV * 1000;
		float rs = ((rand() % (2 * sMagnified)) - sMagnified) * 0.001f;
		forward = forward.rotate(btVector3(1, 0, 0), rs);
	}
	return forward;
}

void Weapon::update()
{

}

ProjectileData * Logic::Weapon::getProjectileData()
{
	return &m_projectileData;
}

int Logic::Weapon::getAmmoCap() { return m_ammoCap; }

void Logic::Weapon::setAmmoCap(int ammoCap) { m_ammoCap = ammoCap; }

int Logic::Weapon::getAmmo() { return m_ammo; }

void Logic::Weapon::setAmmo(int ammo) { m_ammo = ammo; }

int Logic::Weapon::getMagSize() { return m_magSize; }

void Logic::Weapon::setMagSize(int magSize) { m_magSize = magSize; }

int Logic::Weapon::getMagAmmo() { return m_magAmmo; }

void Logic::Weapon::removeMagAmmo() { m_magAmmo--; }

void Logic::Weapon::removeMagAmmo(int ammo) 
{ 
	if (ammo > m_magAmmo)
		m_magAmmo = 0;
	else
		m_magAmmo -= ammo; 
}

int Logic::Weapon::getAmmoConsumption() { return m_ammoConsumption; }

float Logic::Weapon::getAttackTimer()
{
	return (60.f / m_attackRate) * 1000;
}

void Logic::Weapon::fillMag()
{
	int toAdd = m_magSize - m_magAmmo;

	if (m_ammo >= toAdd)
	{
		m_ammo -= toAdd;		// Remove ammo from total
		m_magAmmo = m_magSize;	// Add ammo to mag
	}
	else
	{
		m_magAmmo += m_ammo;	// Add rest of ammo to mag
		m_ammo = 0;				// Remove rest of ammo from total
	}
}
