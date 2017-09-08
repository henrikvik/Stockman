#include "Player/Weapon/Weapon.h"

using namespace Logic;

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
