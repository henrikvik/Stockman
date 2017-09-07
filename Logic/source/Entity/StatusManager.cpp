#include "Entity/StatusManager.h"

using namespace Logic;
 
StatusManager::StatusManager() { }

StatusManager::~StatusManager() { }

void StatusManager::clear()
{
	effects.clear();
	upgrades.clear();
}

void StatusManager::addStatus(int statusID)
{

}

void StatusManager::removeStatus(int statusID)
{

}

std::vector<Effect> StatusManager::getEffects() { return effects; }
std::vector<Upgrade> StatusManager::getUpgrades() { return upgrades; }