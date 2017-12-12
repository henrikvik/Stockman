#include <AI\WaveTimeManager.h>
#include <AI\EntityManager.h>

#include <Keyboard.h>
#include <Singletons\DebugWindow.h>

using namespace Logic;

const float WaveTimeManager::TRANSITION_TIME = 5000.f;

WaveTimeManager::WaveTimeManager()
{
    DebugWindow::getInstance()->registerCommand("SET_WAVE", [&](std::vector<std::string> &para) -> std::string {
        try {
            m_waveCurrent = stoi(para[0]);
            return "Wowie";
        }
        catch (std::exception &ex)
        {
            return "Bigger failure than Ajit Pai";
        }
    });
    reset();
}

WaveTimeManager::~WaveTimeManager()
{
}

void WaveTimeManager::startNextWave(EntityManager &entityManager, btVector3 const &playerPos)
{
    entityManager.spawnWave(m_waveCurrent, playerPos);

    m_timeRequired = entityManager.getWaveManager().getTimeForWave(m_waveCurrent++);
    m_timeCurrent = 0;

    // If the player have completed all the waves
    if (m_waveCurrent == entityManager.getWaveManager().getWaveInformation().nrOfWaves)
        m_onLastWave = true;

    m_onTransition = false;
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
bool WaveTimeManager::update(float deltaTime, EntityManager &entityManager, btVector3 const &playerPos)
{
    m_timeCurrent += deltaTime;

    if (m_onTransition) 
    {
        return updateInTransition(entityManager, playerPos);
    }   
    else
    {
        return updateInWave(entityManager);
    }
}

bool WaveTimeManager::updateInTransition(EntityManager &entityManager, btVector3 const &playerPos)
{
    if (m_timeCurrent < m_timeRequired)
    {
        return false;
    }

    startNextWave(entityManager, playerPos);
    return false; // temp
}

bool WaveTimeManager::updateInWave(EntityManager &entityManager)
{
    int aliveEnemies = entityManager.getNrOfAliveEnemies();
    if (aliveEnemies <= 0 || m_timeCurrent > m_timeRequired)
    {
        if (m_enraged && aliveEnemies > 0)
        {
            // do something fun
        }
        else
        {
            m_enraged = (entityManager.giveEffectToAllEnemies(StatusManager::EFFECT_ID::ENRAGE) > 0);
            if (!m_enraged && !m_onLastWave) {
                startTransition();

                return true;
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

bool WaveTimeManager::getOnLastWave() const
{
    return m_onLastWave;
}

bool WaveTimeManager::onLastWave() const
{
    return m_onLastWave;
}

bool WaveTimeManager::onFirstWave() const
{
    return getCurrentWave() == 0 && isTransitioning();
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
    m_timeCurrent = 0;
}