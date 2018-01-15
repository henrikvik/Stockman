#include <AI\Behavior\EnemyThreadHandler.h>
#include <AI\EntityManager.h>
#include <AI\Enemy.h>
#include <AI\Pathing\AStar.h>
#include <AI\Behavior\Behavior.h>

#include <Singletons\Profiler.h>
#include <thread>
#include <chrono>

using namespace Logic;
const int EnemyThreadHandler::MAX_WORK = 25;

EnemyThreadHandler::EnemyThreadHandler()
{
    resetThreads();
    initThreads();

    enemiesLoaded = timesLoaded = 0;
}

void EnemyThreadHandler::initThreads()
{
    m_killChildren = true;

    for (std::thread *&t : threads)
        if (t)
        {
            if (t->joinable())
                t->join();
            delete t;
        }

    m_killChildren = false;

    while (!m_work.empty())
        m_work.pop();

    for (std::thread *&t : threads)
    {
        t = newd std::thread(&EnemyThreadHandler::threadMain, this);
        SetPriorityClass(t, PROCESS_MODE_BACKGROUND_BEGIN);
    }
}

#include <Misc/FileLoader.h>
EnemyThreadHandler::~EnemyThreadHandler()
{
    deleteThreads();
    std::vector<FileLoader::LoadedStruct> structs;
    FileLoader::LoadedStruct struc;
    for (int i = 0; i < avg.size(); i++)
        struc.floats[std::to_string(i)] = avg[i];
    structs.push_back(struc);
    FileLoader::singleton().saveStructsToFile(structs, "aidata");
}

void EnemyThreadHandler::resetThreads()
{
    for (std::thread *&t : threads)
        t = nullptr;
}

void EnemyThreadHandler::deleteThreads()
{
    m_killChildren = true;
    for (std::thread *&t : threads)
    {
        if (t)
        {
            if (t->joinable())
                t->join();
            delete t;
            t = nullptr;
        }
    }
}

void EnemyThreadHandler::updateEnemiesAndPath(WorkData &data)
{
    const std::vector<Enemy*> &enemies = data.manager->getAliveEnemies()[data.index];
    // make sure it is still enemies in the node before doing all the calculations
    if (!enemies.empty())
    {
        AStar &aStar = AStar::singleton();
        aStar.loadTargetIndex(*data.player);

        std::vector<const DirectX::SimpleMath::Vector3*> path = aStar.getPath(data.index);
        for (size_t i = 0; i < enemies.size(); i++) // (!) size can change throughout the loop (!)
            enemies[i]->getBehavior()->getPath().setPath(path); // TODO: enemy->setPath
    }
}

#include <Singletons\DebugWindow.h>
void EnemyThreadHandler::threadMain()
{
    while (!m_killChildren)
    {
        WorkData todo;
        bool haveWork = false;

        {
            std::lock_guard<std::mutex> lock(m_workMutex);
            if (!m_work.empty())
            {
                todo = m_work.front();
                m_work.pop();
                haveWork = true;

                // TEST
                const std::vector<Enemy*> &enemies = todo.manager->getAliveEnemies()[todo.index];
                if (!enemies.empty()) {
                    enemiesLoaded += enemies.size();
                    timesLoaded++;
                }
            }
        }

        if (haveWork)
            updateEnemiesAndPath(todo);

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void EnemyThreadHandler::addWork(WorkData data)
{
    if (m_work.size() < MAX_WORK)
    {
        std::lock_guard<std::mutex> lock(m_workMutex);
        m_work.push(data);
    }
}

void Logic::EnemyThreadHandler::resetAvg()
{
    if (timesLoaded > 0) {
        float pathsLoadedAvg = enemiesLoaded / float(timesLoaded);
        timesLoaded = 0;
        enemiesLoaded = 0;
        avg.push_back(pathsLoadedAvg);
        DebugWindow::getInstance()->doCommand((std::string("Avg loaded :") + std::to_string(pathsLoadedAvg)).c_str());
    }
}
