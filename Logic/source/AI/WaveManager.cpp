#include <AI/WaveManager.h>
using namespace Logic;

WaveManager::WaveManager(std::string waveFileName)
{
	m_waveFileName = waveFileName;
}


WaveManager::~WaveManager()
{
}

void Logic::WaveManager::setName(std::string name)
{
	m_waveFileName = name;
}

std::string Logic::WaveManager::getWaveFileName() const
{
	return m_waveFileName;
}
