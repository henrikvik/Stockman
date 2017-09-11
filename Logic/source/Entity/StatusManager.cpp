#include "../Entity/StatusManager.h"

using namespace Logic;

Effect StatusManager::s_effects[StatusManager::NR_OF_EFFECTS];
 
StatusManager::StatusManager() { 
	#ifndef buffsCreated
	#define buffsCreated /* THIS IS A TEMPORARY TEST SOLUTION, MOVE TO OTHER CLASS LATER (OR FILE?) */
		Effect creating;

		Effect::Standards standards;
		Effect::Modifiers modifiers;
		Effect::Specifics spec;

		standards.flags = EFFECT_ON_FIRE | EFFECT_MODIFY_MOVEMENTSPEED;
		standards.duration = 3.f;

		creating.setStandards(standards);
		s_effects[ON_FIRE] = creating; // ON FIRE

		standards.flags = EFFECT_MODIFY_DMG_GIVEN;
		standards.duration = 0.f;

		creating.setStandards(standards);
		s_effects[FREEZE] = creating; // FREEZE
	#endif // !buffsCreated
}

StatusManager::~StatusManager() { }

void StatusManager::clear()
{
	m_upgrades.clear();
	m_effectStacks.clear();
	m_effectStacksIds.clear();
}

void StatusManager::removeEffect(int index)
{
	std::swap(m_effectStacks[index], m_effectStacks[m_effectStacks.size()]);
	std::swap(m_effectStacksIds[index], m_effectStacksIds[m_effectStacksIds.size()]);

	m_effectStacks.pop_back();
	m_effectStacksIds.pop_back();
}

void StatusManager::update(float deltaTime)
{
	for (int i = 0; i < NR_OF_EFFECTS; ++i) 
	{
		// do stuff
		if (m_effectStacks[i].duration -= deltaTime <= 0)
		{
			removeEffect(i);
		}
	}
}

void StatusManager::addStatus(int statusID, int nrOfStacks, bool resetDuration)
{
	bool found = false;
	for (size_t i = 0; i < m_effectStacksIds.size() && !found; ++i) 
	{
		if (m_effectStacksIds[i] == statusID) 
		{
			found = true;

			m_effectStacks[i].stack += nrOfStacks;
			if (resetDuration) m_effectStacks[i].duration = s_effects[statusID].getStandards()->duration;
		}
	}

	if (!found) 
	{
		m_effectStacks.push_back({ nrOfStacks, s_effects[statusID].getStandards()->duration });
	}
}

void StatusManager::removeOneStatus(int statusID)
{
	bool found = false;
	for (size_t i = 0; i < m_effectStacksIds.size() && !found; ++i)
	{
		if (m_effectStacksIds[i] == statusID)
		{
			if (m_effectStacks[i].stack-- <= 0) // no more stacks, then remove the effect
			{
				removeEffect(i);
			}
			found = true;
		}
	}
}

void StatusManager::removeAllStatus(int statusID)
{
	bool found = false;
	for (size_t i = 0; i < m_effectStacksIds.size() && !found; ++i)
	{
		if (m_effectStacksIds[i] == statusID)
		{
			removeEffect(i);
			found = true;
		}
	}
}

std::vector<Upgrade*> StatusManager::getUpgrades() { return m_upgrades; }