#include "Entity/Upgrade.h"

using namespace Logic;

Upgrade::Upgrade()
{

}

Upgrade::Upgrade(long long flags, int actionID, FlatUpgrades const &flatUpgrades) 
{
	this->m_flags = flags;
	this->m_actionID = actionID;
	this->m_flatUpgrades = flatUpgrades;
}

void Upgrade::init(long long flags, int actionID, FlatUpgrades const &flatUpgrades)
{
	this->m_flags = flags;
	this->m_actionID = actionID;
	this->m_flatUpgrades = flatUpgrades;
}

Upgrade::~Upgrade() { }

Upgrade::FlatUpgrades Upgrade::getFlatUpgrades() const
{
	return m_flatUpgrades;
}

std::vector<int> Upgrade::getTranferEffects() const
{
	return m_tranferEffects;
}