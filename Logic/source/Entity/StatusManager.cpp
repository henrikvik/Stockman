#include "../Entity/StatusManager.h"

using namespace Logic;
 
StatusManager::StatusManager() { }

StatusManager::~StatusManager() { }

void StatusManager::clear()
{
	m_effects.clear();
	m_upgrades.clear();
}

void StatusManager::addStatus(int statusID)
{

}

void StatusManager::removeStatus(int statusID)
{

}

std::vector<Effect> StatusManager::getEffects() { return m_effects; }
std::vector<Upgrade> StatusManager::getUpgrades() { return m_upgrades; }