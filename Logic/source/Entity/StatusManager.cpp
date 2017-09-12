#include <Entity\StatusManager.h>
#include <stdio.h>

using namespace Logic;

Effect StatusManager::s_effects[StatusManager::NR_OF_EFFECTS];
Upgrade StatusManager::s_upgrades[StatusManager::NR_OF_EFFECTS];
 
StatusManager::StatusManager() 
{ 
	#ifndef BUFFS_CREATED
	#define BUFFS_CREATED
	/* THIS IS A TEMPORARY TEST SOLUTION, MOVE TO OTHER CLASS LATER (OR FILE?) */
		Effect creating;

		Effect::Standards standards;
		Effect::Modifiers modifiers;
		Effect::Specifics spec;

		standards.flags = Effect::EFFECT_ON_FIRE | Effect::EFFECT_MODIFY_MOVEMENTSPEED;
		standards.duration = 300.f;

		creating.setStandards(standards);
		s_effects[ON_FIRE] = creating; // ON FIRE

		standards.flags = Effect::EFFECT_MODIFY_DMG_GIVEN;
		standards.duration = 0.f;

		creating.setStandards(standards);
		s_effects[FREEZE] = creating; // FREEZE
	#endif // !buffsCreated

	#ifndef UPGRADES_CREATED
	#define UPGRADES_CREATED
										  /* THIS IS A TEMPORARY TEST SOLUTION, MOVE TO OTHER CLASS LATER (OR FILE?) */
			Upgrade upgrade;
			Upgrade::FlatUpgrades flat;
			memset(&flat, 0, sizeof(flat));

			upgrade.init(Upgrade::UPGRADE_IS_WEAPON | Upgrade::UPGRADE_IS_BOUNCING,
						 0, flat);
			s_upgrades[BOUNCE] = upgrade;
			memset(&flat, 0, sizeof(flat));

			flat.increaseAmmoCap = 10;
			upgrade.init(Upgrade::UPGRADE_IS_WEAPON | Upgrade::UPGRADE_INCREASE_AMMOCAP,
						0, flat);
			s_upgrades[P10_AMMO] = upgrade; // FREEZE
			memset(&flat, 0, sizeof(flat));
	#endif // !UPGRADES_CREATED

	addStatus(0, 5, true);
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
	std::swap(m_effectStacks[index], m_effectStacks[m_effectStacks.size() - 1]);
	std::swap(m_effectStacksIds[index], m_effectStacksIds[m_effectStacksIds.size() - 1]);

	m_effectStacks.pop_back();
	m_effectStacksIds.pop_back();
}

void StatusManager::update(float deltaTime)
{
	for (size_t i = 0; i < m_effectStacks.size(); ++i)
	{
		// do stuff
		printf("Dur: %f, DT: %f\n", m_effectStacks[i].duration, deltaTime);
		if ((m_effectStacks[i].duration -= deltaTime) <= 0)
		{
			removeEffect(i);
		}
	}
}

void StatusManager::addUpgrade(UPGRADE_ID id) 
{
	m_upgrades.push_back(Upgrade(s_upgrades[id]));
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
			if (resetDuration) m_effectStacks[i].duration =
				s_effects[statusID].getStandards()->duration;
		}
	}

	if (!found)
	{
		m_effectStacks.push_back({ nrOfStacks,
								s_effects[statusID].getStandards()->duration });
		m_effectStacksIds.push_back(statusID);
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

std::vector<Upgrade>* StatusManager::getUpgrades() { return &m_upgrades; }