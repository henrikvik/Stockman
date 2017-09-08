#include <AI/WaveManager.h>
using namespace Logic;

WaveManager::WaveManager(std::string waveFileName)
{
	m_waveFileName = waveFileName;
}


WaveManager::~WaveManager()
{
}

std::vector<int> Logic::WaveManager::getEnemies(int waveId)
{
	std::vector<int> enemies;
	enemies.resize(0);
	return enemies;
}

void Logic::WaveManager::setName(std::string name)
{
	m_waveFileName = name;
}

std::string Logic::WaveManager::getWaveFileName() const
{
	return m_waveFileName;
}
