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
}

WaveManager::EntitiesInWave WaveManager::getEntities(int waveId)
{
	EntitiesInWave entities;
	std::string item;
	int currentWave = 1;

	FileLoader::LoadedStruct struc;
	for (int i = 0; i < m_waveFileLoaded.size(); i++)
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

WaveManager::Entity WaveManager::loadEntity(int index, int offset)
{
	FileLoader::LoadedStruct struc = m_waveFileLoaded[index + offset];
	
	return {struc.ints["id"], struc.floats["x"],
		struc.floats["y"], struc.floats["z"],
		splitInts(struc.strings["effects"], FILE_DELIM) };
}

std::vector<int> WaveManager::splitInts(std::string const &item, const char delim)
{
	std::vector<int> list;
	std::string itemPart;
	strstream.str(item);

	while (std::getline(strstream, itemPart, delim)) 
		list.push_back(std::stoi(itemPart));

	if (list.empty() && !item.empty())
		list.push_back(std::stoi(item));

	return list;
}