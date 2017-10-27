#include <AI\Behavior\EnemyThreadHandler.h>

#include <AI\EntityManager.h>
#include <AI\Enemy.h>
#include <AI\Behavior\AStar.h>
#include <AI\Behavior\Behavior.h>

using namespace Logic;

EnemyThreadHandler::EnemyThreadHandler()
{
    ZeroMemory(&m_threadRunning, sizeof(m_threadRunning));
    ZeroMemory(&m_indexRunning,  sizeof(m_indexRunning));
    resetThreads();
}

EnemyThreadHandler::~EnemyThreadHandler()
{
    deleteThreads();
}

void EnemyThreadHandler::resetThreads()
{
    for (std::thread *&t : threads)
        t = nullptr;
}

void EnemyThreadHandler::deleteThreads()
{
    m_killChildren = true;
    for (std::thread *t : threads)
    {
        if (t)
        {
            if (t->joinable())
                t->join();
            delete t;
        }
    }
}

void EnemyThreadHandler::updateEnemiesAndPath(WorkData &data)
{
    AStar &aStar = AStar::singleton();
    aStar.loadTargetIndex(data.player);

    std::vector<const DirectX::SimpleMath::Vector3*> path = aStar.getPath(data.index);
    const std::vector<Enemy*> &enemies = data.manager->getAliveEnemies()[data.index];

    for (size_t i = 0; i < enemies.size(); i++) // (!) size can change throughout the loop (!)
        enemies[i]->getBehavior()->getPath().setPath(path); // TODO: enemy->setPath

    m_threadRunning[getThreadId(data.index)] = false;
}

void EnemyThreadHandler::threadMain(WorkData data)
{
    while (!m_killChildren)
    {
        if (m_threadRunning[getThreadId(data.index)])
            updateEnemiesAndPath(data);
        std::this_thread::sleep_for(2ms);
    }
}

void EnemyThreadHandler::addWork(WorkData data)
{
    if (getThreadStatus(data.index) & OPEN)
    {
        int thread = getThreadId(data.index);
        m_threadRunning[thread] = true;
        std::thread *t = threads[thread];

        if (t)
            m_indexRunning[thread] = data.index;
        else
        {
            PROFILE_BEGIN("Create Thread");
            t = newd std::thread(&EnemyThreadHandler::threadMain, this, data);
            threads[thread] = t;
            PROFILE_END();
        }
    }
}

int EnemyThreadHandler::getThreadStatus(int i) {
    int ret = 0, threadId = getThreadId(i);

    if (m_threadRunning[threadId] && m_indexRunning[threadId] == i)
        ret |= LOCKED & RUNNING;
    else if (m_threadRunning[threadId])
        ret |= LOCKED;
    else 
        ret |= OPEN;

    return ret;
}