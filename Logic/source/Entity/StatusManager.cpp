#include <Entity\StatusManager.h>
#include <Entity\Entity.h>

#include <stdio.h>
#include <Misc/FileLoader.h>

#define FILE_NAME_EFFECTS "Effects"
#define FILE_NAME_UPGRADES "Upgrades"

using namespace Logic;

Effect StatusManager::s_effects[StatusManager::NR_OF_EFFECTS];
Upgrade StatusManager::s_upgrades[StatusManager::NR_OF_UPGRADES];
 
StatusManager::StatusManager() 
{ 
    static bool statusesCreated = false;
    if (!statusesCreated)
    {
        statusesCreated = true;
        {
            // CREATE UPGRADES
            long long flags;
            Upgrade::FlatUpgrades flat;
            std::vector<FileLoader::LoadedStruct> loadedUpgrades;

            FileLoader::singleton().loadStructsFromFile(loadedUpgrades, FILE_NAME_UPGRADES);
            int id = 0;

            for (auto const &fileStruct : loadedUpgrades)
            {
                flags = fileStruct.ints.at("flags");
                flat.increaseCooldown = fileStruct.floats.at("increaseCooldown");
                flat.increaseDmg = fileStruct.ints.at("increaseDamage");
                flat.increaseAmmoCap = fileStruct.ints.at("increaseAmmoCap");
                flat.increaseMagSize = fileStruct.ints.at("increaseMagSize");
                flat.increaseSize = fileStruct.ints.at("increaseSize");
                s_upgrades[id].init(flags, id, flat);
                id++;
            }
        }

        {
            // CREATE EFFECTS
            std::vector<FileLoader::LoadedStruct> loadedEffects;
            FileLoader::singleton().loadStructsFromFile(loadedEffects, FILE_NAME_EFFECTS);
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

                    modifiers.modifyDmgGiven = fileStruct.floats.at("mDmgGiven");
                    modifiers.modifyDmgTaken = fileStruct.floats.at("mDmgTaken");
                    modifiers.modifyFirerate = fileStruct.floats.at("mFirerate");
                    modifiers.modifyHP = fileStruct.floats.at("mHP");
                    modifiers.modifyMovementSpeed = fileStruct.floats.at("mMovementSpeed");

                    creating.setModifiers(modifiers);
                }

                if (fileStruct.ints.at("specifics"))
                {
                    memset(&spec, 0, sizeof(spec));

                    spec.isBulletTime = fileStruct.floats.at("sBulletTime");
                    spec.isFreezing = fileStruct.floats.at("sFreezing");
                    spec.ammoType = fileStruct.floats.at("sAmmoType");

                    creating.setSpecifics(spec);
                }

                creating.setStandards(standards);
                s_effects[id] = creating;
                id++;
            }
        }
    }
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

void StatusManager::update(float deltaTime, Entity &entity)
{
    // two sep lists maybe is redundant, fix?
    int id, stacks;
	for (size_t i = 0; i < m_effectStacks.size(); ++i)
	{
        id = static_cast<int> (i);
        stacks = m_effectStacks[id].stack;

        Effect &effect = s_effects[m_effectStacksIds[id]];
        entity.affect(stacks, effect, deltaTime);

		if ((m_effectStacks[i].duration -= deltaTime) <= 0)
		{
            entity.onEffectEnd(stacks, effect);
			removeEffect(id);
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
				removeEffect((int)i);
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
			removeEffect((int)i);
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
	int size = (int)m_effectStacks.size();
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

	int size = (int)m_effectStacks.size();
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
