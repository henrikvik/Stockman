#include "Player/Weapon/Weapon.h"

using namespace Logic;

Weapon::Weapon()
{
	m_weaponID		= -1;
	m_ammoCap		= -1;
	m_magSize		= -1;
	m_damage		= -1;
	m_attackRate	= -1;
	m_freeze		= -1;
}

Weapon::Weapon(int weaponID, int ammoCap, int magSize, float damage, float attackRate, float freeze)
{
	m_weaponID		= weaponID;
	m_ammoCap		= ammoCap;
	m_magSize		= magSize;
	m_damage		= damage;
	m_attackRate	= attackRate;
	m_freeze		= freeze;
}

void Weapon::use()
{
}

void Weapon::update()
{

}
