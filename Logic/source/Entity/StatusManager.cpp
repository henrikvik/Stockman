#include <Entity\StatusManager.h>
#include <stdio.h>
#include <Misc/FileLoader.h>

#define FILE_NAME "Effects"

using namespace Logic;

Effect StatusManager::s_effects[StatusManager::NR_OF_EFFECTS];
Upgrade StatusManager::s_upgrades[StatusManager::NR_OF_UPGRADES];
 
StatusManager::StatusManager() 
{ 
	#ifndef BUFFS_CREATED
	#define BUFFS_CREATED
		std::vector<FileLoader::LoadedStruct> loadedEffects;
		FileLoader::singleton().loadStructsFromFile(loadedEffects, FILE_NAME);
		Effect::Standards standards;
		Effect::Modifiers modifiers;
		Effect::Specifics spec;
		int id = 0;

		for (auto const &fileStruct : loadedEffects)
		{
			Effect creating;

			standards.flags = fileStruct.ints.at("flags");
			standards.duration = fileStruct.floats.at("duration");

			if (fileStruct.ints.at("modifiers"))
			{
				memset(&modifiers, 0, sizeof(modifiers));

				modifiers.modifyDmgGiven =		fileStruct.floats.at("mDmgGiven");
				modifiers.modifyDmgTaken =		fileStruct.floats.at("mDmgTaken");
				modifiers.modifyFirerate =		fileStruct.floats.at("mFirerate");
				modifiers.modifyHP =			fileStruct.floats.at("mHP");
				modifiers.modifyMovementSpeed = fileStruct.floats.at("mMovementSpeed");

				creating.setModifiers(modifiers);
			}

			if (fileStruct.ints.at("specifics"))
			{
				memset(&spec, 0, sizeof(spec));
			
				spec.isBulletTime = fileStruct.floats.at("sBulletTime");
				spec.isFreezing =	fileStruct.floats.at("sFreezing");

				creating.setSpecifics(spec);
			}

			creating.setStandards(standards);
			s_effects[id++] = creating;
		}
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
}

StatusManager::~StatusManager() {
	clear();
}

void StatusManager::clear()
{
	m_upgrades.clear();
	m_effectStacks.clear();
	m_effectStacksIds.clear();
}

int StatusManager::getStacksOfEffectFlag(Effect::EFFECT_FLAG flag) const
{
	int stacks = 0;
	for (size_t i = 0; i < m_effectStacksIds.size(); ++i)
	{
		if (s_effects[m_effectStacksIds[i]].getStandards()->flags & flag)
			stacks += m_effectStacks[i].stack;
	}
	return stacks;
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
		if ((m_effectStacks[i].duration -= deltaTime) <= 0)
		{
			removeEffect(i);
		}
	}
}

void StatusManager::addUpgrade(UPGRADE_ID id) 
{
	m_upgrades.push_back(id);
}

Upgrade & Logic::StatusManager::getUpgrade(UPGRADE_ID id)
{
	return s_upgrades[id];
}

const Effect& StatusManager::getEffect(EFFECT_ID id) const
{
	return s_effects[id];
}

void StatusManager::addStatus(StatusManager::EFFECT_ID effectID, int nrOfStacks, bool resetDuration)
{
	bool found = false;
	for (size_t i = 0; i < m_effectStacksIds.size() && !found; ++i)
	{
		if (m_effectStacksIds[i] == effectID)
		{
			found = true;

			m_effectStacks[i].stack += nrOfStacks;
			if (resetDuration) m_effectStacks[i].duration =
				s_effects[effectID].getStandards()->duration;
		}
	}

	if (!found)
	{
		m_effectStacks.push_back({ nrOfStacks,
								s_effects[effectID].getStandards()->duration });
		m_effectStacksIds.push_back(effectID);
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

std::vector <std::pair<int, Effect*>>
	StatusManager::getActiveEffects() 
{
	// this is per frame allocation, kind of bad,
	// should be changed but this is just to test
	// the effects.

	// For better ways to do this in the future see
	// mike acton ty
	int size = m_effectStacks.size();
	std::vector<std::pair<int, Effect*>> actives;
	actives.resize(size);

	size_t i = 0; // OPTIMIZE!
	for (i = 0; i < size; ++i)
		actives[i].first = m_effectStacks[i].stack;
	for (i = 0; i < size; ++i)
		actives[i].second = &s_effects[m_effectStacksIds[i]];

	return actives;
}

std::vector<std::pair<int, StatusManager::EFFECT_ID>> StatusManager::getActiveEffectsIDs()
{
	std::vector<std::pair<int, StatusManager::EFFECT_ID>> effects;

	int size = m_effectStacks.size();
	effects.resize(size);

	for (size_t i = 0; i < size; ++i)
		effects[i].first = m_effectStacks[i].stack;
	for (size_t i = 0; i < size; ++i)
		effects[i].second = m_effectStacksIds[i];

	return effects;
}

std::vector<StatusManager::UPGRADE_ID>& StatusManager::getActiveUpgrades()
{
	return m_upgrades;
}
