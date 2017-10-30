#include <AI/WaveManager.h>

#define FILE_ENEMIES "enemies"
#define FILE_TRIGGERS "triggers"
#define FILE_BOSSES "bosses"
#define FILE_DELIM ','

using namespace Logic;

WaveManager::WaveManager(std::string waveFileName)
{
	m_waveFileName = waveFileName;
}

WaveManager::~WaveManager()
{
}

void WaveManager::loadFile()
{
	FileLoader::singleton().loadStructsFromFile(m_waveFileLoaded, m_waveFileName);

    FileLoader::LoadedStruct waveInfo = m_waveFileLoaded[0];
    m_waveInformation.nrOfWaves = waveInfo.ints["nrOfWaves"];
    m_waveInformation.startWave = waveInfo.ints["startWave"];
    m_waveInformation.timeMul   = waveInfo.floats["timeMul"];
    m_waveInformation.timeBonus = waveInfo.floats["timeBonus"];
    m_waveInformation.bossWaves = splitInts(waveInfo.strings["bossWaves"], FILE_DELIM);
    m_waveInformation.bossTimeBonus = waveInfo.floats["bossTimeBonus"];
}

WaveManager::EntitiesInWave WaveManager::getEntities(int waveId) const
{
	EntitiesInWave entities;
	std::string item;
	int currentWave = 0;

	FileLoader::LoadedStruct struc;
	for (int i = 1; i < m_waveFileLoaded.size(); i++)
	{
		if (currentWave == waveId)
		{
			struc = m_waveFileLoaded[i];
			entities.enemies = splitInts(struc.strings[FILE_ENEMIES], FILE_DELIM);

			for (int j = 0; j < m_waveFileLoaded[i].ints[FILE_TRIGGERS];
				j++) // load triggers
				entities.triggers.push_back(loadEntity(i, j + 1));

			for (int k = 0; k < m_waveFileLoaded[i].ints[FILE_ENEMIES]; k++)
				entities.bosses.push_back(
					loadEntity(i + m_waveFileLoaded[i].ints[FILE_TRIGGERS], k));
		}
		else
		{
			i += struc.ints[FILE_BOSSES] + struc.ints[FILE_TRIGGERS];
		}
		currentWave++;
	}

	return entities;
}

void WaveManager::setName(std::string name)
{
	m_waveFileName = name;
}

std::string WaveManager::getWaveFileName() const
{
	return m_waveFileName;
}

WaveManager::Entity WaveManager::loadEntity(int index, int offset) const
{
	FileLoader::LoadedStruct struc = m_waveFileLoaded[index + offset];
	
	return {struc.ints["id"], struc.floats["x"],
		struc.floats["y"], struc.floats["z"],
		splitInts(struc.strings["effects"], FILE_DELIM) };
}

std::vector<int> WaveManager::splitInts(std::string const &item, const char delim) const
{
	std::vector<int> list;
	std::string itemPart;
	std::stringstream strstream(item);

	while (std::getline(strstream, itemPart, delim)) 
		list.push_back(std::stoi(itemPart));

	if (list.empty() && !item.empty())
		list.push_back(std::stoi(item));

	return list;
}

WaveManager::WaveInformation WaveManager::getWaveInformation() const
{
    return m_waveInformation;
}

float WaveManager::getTimeForWave(int waveId) const
{
    EntitiesInWave enInWave = getEntities(waveId);
    return m_waveInformation.timeBonus + m_waveInformation.timeMul *
        (enInWave.enemies.size() + enInWave.bosses.size() * m_waveInformation.bossTimeBonus);
}
