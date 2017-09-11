#include "../Entity/StatusManager.h"

using namespace Logic;
 
StatusManager::StatusManager() { 
#ifndef buffsCreated
#define buffsCreated /* THIS IS A TEMPORARY TEST SOLUTION, MOVE TO OTHER CLASS LATER (OR FILE?) */
	Effect::Standards standards;
	Effect::Modifiers modifiers;
	Effect::Specifics spec;

	standards.flags = EFFECT_ON_FIRE | EFFECT_MODIFY_MOVEMENTSPEED;
	standards.duration = 3.f;

	s_onFire.setStandards(standards);

	standards.flags = EFFECT_MODIFY_DMG_GIVEN;
	standards.duration = 0.f;

	s_freeze.setStandards(standards);

	s_effects[ON_FIRE] = &s_onFire;
	s_effects[FREEZE] = &s_freeze;
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
	m_effects.push_back(std::pair<int, Effect*>(
		statusID, new Effect(*s_effects[statusID])));
}

void StatusManager::removeOneStatus(int statusID)
{
	for (int i = 0; i < m_effects.size(); i++)
	{
		if (m_effects[i].first == statusID) 
		{
			delete m_effects[i].second;
			std::swap(m_effects[i], m_effects[m_effects.size()]);
			m_effects.pop_back();
		}
	}
}

void StatusManager::removeAllStatus(int statusID)
{
	for (int i = 0; i < m_effects.size(); i++)
	{
		if (m_effects[i].first == statusID)
		{
			m_effects[i].second->pop();
			if (m_effects[i].second->getStack() <= 0)
			{
				delete m_effects[i].second;
				std::swap(m_effects[i], m_effects[m_effects.size()]);
				m_effects.pop_back();
			}
		}
	}
}

std::vector<std::pair<int, Effect*>> StatusManager::getEffects() { return m_effects; }
std::vector<Upgrade*> StatusManager::getUpgrades() { return m_upgrades; }