#include <AI/WaveManager.h>
using namespace Logic;

WaveManager::WaveManager(std::string waveFileName)
{
	m_waveFileName = waveFileName;
}

WaveManager::~WaveManager()
{
}

std::vector<int> WaveManager::getEnemies(int waveId)
{
	std::vector<int> enemies;
	enemies.resize(10);
	return enemies;
}

void WaveManager::setName(std::string name)
{
	m_waveFileName = name;
}

std::string WaveManager::getWaveFileName() const
{
	return m_waveFileName;
}
