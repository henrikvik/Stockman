#include "../Entity/StatusManager.h"

using namespace Logic;
 
StatusManager::StatusManager() { 
#ifndef buffsCreated
#define buffsCreated /* THIS IS A TEMPORARY TEST SOLUTION, MOVE TO OTHER CLASS LATER */
	Effect::Standards standards;
	Effect::Modifiers modifiers;
	Effect::Specifics spec;

	standards.flags = EFFECT_ON_FIRE | EFFECT_MODIFY_MOVEMENTSPEED;
	standards.duration = 3;

	onFire.setStandards(standards);

	standards.flags = EFFECT_MODIFY_DMG_GIVEN;
	standards.duration = 0;

	freeze.setStandards(standards);
#endif // !buffsCreated
}

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

std::vector<Effect*> StatusManager::getEffects() { return m_effects; }
std::vector<Upgrade*> StatusManager::getUpgrades() { return m_upgrades; }