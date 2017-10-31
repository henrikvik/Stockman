#include <AI\WaveTimeManager.h>
#include <AI\EntityManager.h>
using namespace Logic;

WaveTimeManager::WaveTimeManager()
{
    m_timeCurrent = 0.f;
    m_timeRequired = 0.f;

    m_onLastWave = false;
    m_enraged = false;
}


WaveTimeManager::~WaveTimeManager()
{
}

void WaveTimeManager::update(float deltaTime, EntityManager &entityManager)
{
    if (!m_onLastWave)
    {
        m_timeCurrent += deltaTime;
        if (m_timeCurrent > m_timeRequired || entityManager.getNrOfAliveEnemies() == 0)
        {
            if (m_enraged && entityManager.getNrOfAliveEnemies() > 0)
            {
                // do something fun
            } 
            else
            {
                m_enraged = entityManager.giveEffectToAllEnemies(StatusManager::EFFECT_ID::ENRAGE) > 0;
                if (!m_enraged) {
                    entityManager.spawnWave(m_waveCurrent++);

                    m_timeRequired = entityManager.getWaveManager().getTimeForWave(m_waveCurrent);
                    m_timeCurrent = 0;

                    // If the player have completed all the waves
                    if (m_waveCurrent == entityManager.getWaveManager().getWaveInformation().nrOfWaves)
                        m_onLastWave = true;
                }
            }
        }
    }
}

int WaveTimeManager::getCurrentWave() const
{
    return m_waveCurrent;
}

float WaveTimeManager::getTimeCurrent() const
{
    return m_timeCurrent;
}

float WaveTimeManager::getTimeRequired() const
{
    return m_timeRequired;
}

bool WaveTimeManager::onLastWave() const
{
    return m_onLastWave;
}
