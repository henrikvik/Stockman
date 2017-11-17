#include <AI\WaveTimeManager.h>
#include <AI\EntityManager.h>
using namespace Logic;

const float WaveTimeManager::TRANSITION_TIME = 10000.f;

WaveTimeManager::WaveTimeManager()
{
    reset();
}


WaveTimeManager::~WaveTimeManager()
{
}

void WaveTimeManager::reset()
{
    m_waveCurrent = 0;

    m_timeCurrent = 0.f;

    m_onLastWave = false;
    m_enraged = false;

    startTransition();
}

// Returns true if a transition is happening
bool WaveTimeManager::update(float deltaTime, EntityManager &entityManager)
{
    if (!m_onLastWave)
    {
        m_timeCurrent += deltaTime;

        if (m_onTransition) 
        {
            if (m_timeCurrent > m_timeRequired)
            {
                entityManager.deallocateData(false);
                entityManager.spawnWave(m_waveCurrent++);

                m_timeRequired = entityManager.getWaveManager().getTimeForWave(m_waveCurrent);
                m_timeCurrent = 0;

                // If the player have completed all the waves
                if (m_waveCurrent == entityManager.getWaveManager().getWaveInformation().nrOfWaves)
                    m_onLastWave = true;

                m_onTransition = false;
            }
        }
        else
        {
            if (entityManager.getNrOfAliveEnemies() == 0 || m_timeCurrent > m_timeRequired)
            {
                if (m_enraged && entityManager.getNrOfAliveEnemies() > 0)
                {
                    // do something fun
                }
                else
                {
                    m_enraged = entityManager.giveEffectToAllEnemies(StatusManager::EFFECT_ID::ENRAGE) > 0;
                    if (!m_enraged) {
                        m_onTransition = true;

                        startTransition();

                        return true;
                    }
                }
            }
        }
    }

    return false;
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

bool WaveTimeManager::isEnraged() const
{
    return m_enraged;
}

bool WaveTimeManager::isTransitioning() const
{
    return m_onTransition;
}

void WaveTimeManager::startTransition()
{
    m_onTransition = true;
    m_timeRequired = TRANSITION_TIME;
    m_waveCurrent = 0;
}