#include <AI/EntityManager.h>
using namespace Logic;

#define FILE_ABOUT_WHALES "Enemies/Wave"

#include <AI\Behavior\EnemyThreadHandler.h>
#include <AI\Pathing\AStar.h>
#include <AI\EnemyTest.h>
#include <AI\EnemyNecromancer.h>
#include <AI\EnemyBossBaddie.h>
#include <AI\EnemySoarer.h>
#include <AI\EnemyChaser.h>
#include <Misc\ComboMachine.h>

#include <Player\Player.h>
#include <Projectile\ProjectileManager.h>

#include <Graphics\include\Structs.h>
#include <Physics\Physics.h>
#include <Graphics\include\Particles\ParticleSystem.h>

#include <DebugDefines.h>
#include <Misc\RandomGenerator.h>

#include <Engine\DebugWindow.h>
#include <Engine\Profiler.h>

#include <ctime>
#include <stdio.h>

const int EntityManager::NR_OF_THREADS = 4, EntityManager::ENEMY_CAP = 65;
int EntityManager::PATH_UPDATE_DIV = 25;

EntityManager::EntityManager()
{
    m_frame = 0;
    m_aliveEnemies = 0;
    m_aiType = NORMAL_MODE;

    m_automaticTesting = false;
    m_debugPath = false;

    //allocateData(); was here
    registerCreationFunctions();
    loadDebugCmds();

    m_waveManager.setName(FILE_ABOUT_WHALES);
    m_waveManager.loadFile();
}

EntityManager::~EntityManager()
{
    deallocateData();
    delete m_threadHandler;
}

void EntityManager::registerCreationFunctions()
{
    m_enemyFactory[EnemyType::NECROMANCER] = [](btVector3 const &pos, float scale, std::vector<int> const &effects, Physics &physics) -> Enemy*
    {
        Cube cube(pos, { 0.f, 0.f, 0.f }, (btVector3{ 0.8f, 1.5f, 0.8f } * btScalar(scale)));
        btRigidBody *body = physics.createBody(cube, 100, false,
            Physics::COL_ENEMY, (Physics::COL_EVERYTHING));
        body->setAngularFactor(btVector3(0, 1, 0));

        Enemy* enemy = newd EnemyNecromancer(body, cube.getDimensionsRef());
        body = physics.createBody(Cube({ 0, 0, 0 }, { 0, 0, 0 }, { 1.f, 1.f, 1.f }),
            0.f, true, Physics::COL_ENEMY, (Physics::COL_EVERYTHING));
        physics.removeRigidBody(body);
        enemy->addExtraBody(body, 2.f, { 0.f, 3.f, 0.f });

        return enemy;
    };
    m_enemyFactory[EnemyType::NECROMANCER_MINION] = [](btVector3 const &pos, float scale, std::vector<int> const &effects, Physics &physics) -> Enemy*
    {
        Cube cube(pos, { 0.f, 0.f, 0.f }, (btVector3{ 1.f, 1.f, 0.6f } * btScalar(scale)));
        btRigidBody *body = physics.createBody(cube, 100, false,
            Physics::COL_ENEMY, (Physics::COL_EVERYTHING));
        body->setAngularFactor(btVector3(0, 1, 0));

        Enemy* enemy = newd EnemyChaser(body);

        return enemy;
    };
    m_enemyFactory[EnemyType::BOSS_1] = [](btVector3 const &pos, float scale, std::vector<int> const &effects, Physics &physics) -> Enemy*
    {
        Cube cube(pos, { 0.f, 0.f, 0.f }, (btVector3{ 1.9f, 4.5f, 1.9f } * btScalar(scale)));
        btRigidBody *body = physics.createBody(cube, 100, false,
            Physics::COL_ENEMY, (Physics::COL_EVERYTHING));
        body->setAngularFactor(btVector3(0, 1, 0));

        Enemy* enemy = newd EnemyBossBaddie(body, cube.getDimensionsRef());
        body = physics.createBody(Cube({ 0, 0, 0 }, { 0, 0, 0 }, { 1.f, 1.f, 1.f }),
            0.f, true, Physics::COL_ENEMY, (Physics::COL_EVERYTHING));
        physics.removeRigidBody(body);
        enemy->addExtraBody(body, 2.f, { 0.f, 3.f, 0.f });

        return enemy;
    };
    m_enemyFactory[EnemyType::FLYING] = [](btVector3 const &pos, float scale, std::vector<int> const &effects, Physics &physics) -> Enemy*
    {
        Cube cube(pos, { 0.f, 0.f, 0.f }, (btVector3{ 4.2f, 2.2f, 4.2f } * btScalar(scale)));
        btRigidBody *body = physics.createBody(cube, 100, false,
            Physics::COL_ENEMY, (Physics::COL_EVERYTHING));
        body->setAngularFactor(btVector3(0, 1, 0));

        Enemy* enemy = newd EnemySoarer(body, cube.getDimensionsRef());
        body = physics.createBody(Cube({ 0, 0, 0 }, { 0, 0, 0 }, { 1.f, 1.f, 1.f }),
            0.f, true, Physics::COL_ENEMY, (Physics::COL_EVERYTHING));
        physics.removeRigidBody(body);
        enemy->addExtraBody(body, 2.f, { 0.f, 3.f, 0.f });

        return enemy;
    };
}

void EntityManager::resetTriggers()
{
    m_triggerManager.reset();
}

void EntityManager::allocateData()
{
    m_enemies.resize(AStar::singleton().getNrOfPolygons());
    m_threadHandler = newd EnemyThreadHandler();

    for (auto &list : m_enemies)
        list.reserve(ENEMY_CAP);
}

void EntityManager::loadDebugCmds()
{
#ifdef _DEBUG
    DebugWindow::getInstance()->registerCommand("AI_SETMODE", [&](std::vector<std::string> &para) -> std::string {
        try {
            m_aiType = static_cast<AIType> (stoi(para[0]));
            return "AI Mode Updated";
        } catch (std::exception e) {
            return "No, Chaos is a ladder.";
        }
    });
#endif
    DebugWindow::getInstance()->registerCommand("AI_AUTOMATIC_TESTING", [&](std::vector<std::string> &para) -> std::string {
        m_automaticTesting = true;
        PATH_UPDATE_DIV = 1;
        return "TESTING ACTIVED (QUIT TO TURN OFF)";
    });
    DebugWindow::getInstance()->registerCommand("AI_TOGGLE_PATH", [&](std::vector<std::string> &para) -> std::string {
        m_debugPath = !m_debugPath;
        return "Path Toggled";
    });
    DebugWindow::getInstance()->registerCommand("AI_SPAWN_ENEMY", [&](std::vector<std::string> &para) -> std::string {
        try {
            RandomGenerator &generator = RandomGenerator::singleton();
            btVector3 pos = pos = { generator.getRandomFloat(-85, 85), generator.getRandomFloat(10, 25),
                generator.getRandomFloat(-85, 85) };
            SpawnEnemy(static_cast<EnemyType> (stoi(para[0])), pos, {});
            return "Enemy spawned";
        }
        catch (std::exception e) {
            return "DOTHRAKI IN THE OPEN FIELD, NED";
        }
    });
}

void EntityManager::deallocateData(bool forceDestroy)
{
    m_threadHandler->initThreads(); // todo: better sol

    for (std::vector<Enemy*>& list : m_enemies)
    {
        for (Enemy *&enemy : list)
        {
            if (forceDestroy || !enemy->hasCallbackEntities())
            {
                if (forceDestroy)
                    enemy->clearCallbacks(false);
                DeleteBody(*enemy);
                delete enemy;
                enemy = nullptr;
            }
            else {
                m_deadEnemies.push_back(enemy);
            }
        }
        list.clear();
        list.reserve(ENEMY_CAP);
    }

    // clear out enemies without callbacks in needs to do
    for (Enemy *&enemy : m_deadEnemies)
    {
        if (forceDestroy || !enemy->hasCallbackEntities()) 
        {
            if (forceDestroy)
                enemy->clearCallbacks(false);
            delete enemy;
            enemy = nullptr;
        }
    }

    // copy over old list without destroyed enemies
    std::vector<Enemy*> oldList = m_deadEnemies;
    m_deadEnemies.clear();
    for (Enemy *enemy : oldList)
        if (enemy)
            m_deadEnemies.push_back(enemy);

    m_aliveEnemies = 0;
}

void EntityManager::update(Player &player, float deltaTime)
{
	m_frame++;
	m_deltaTime = deltaTime;
	
	PROFILE_BEGIN("EntityManager::update()");
    if (m_aiType != NO_AI_MODE)
    {
        for (size_t i = 0; i < m_enemies.size(); i++)
        {
            if (m_enemies[i].size() > 0)
            {
                updateEnemies(static_cast<int> (i), player, deltaTime);
                if ((i + m_frame) % PATH_UPDATE_DIV == 0)
                    m_threadHandler->addWork({ this, static_cast<int> (i), &player });
            }
        }
    }
	PROFILE_END();

	for (int i = 0; i < m_deadEnemies.size(); ++i)
	{
	    m_deadEnemies[i]->updateDead(deltaTime); // Delete body right now, maybe later keep the dead bodies, but it is not really important but can be changed
	}

	m_triggerManager.update(deltaTime);

    if (m_automaticTesting) automaticUpdate(player);
}

void EntityManager::updateEnemies(int index, Player &player, float deltaTime)
{
	bool goalNodeChanged = false;
    std::vector<Enemy*> &enemies = m_enemies[index];
	
	for (size_t i = 0; i < enemies.size(); ++i)
	{
        updateEnemy(enemies[i], enemies, (int)i, index, player, deltaTime, true);
	}
}

void EntityManager::updateEnemy(Enemy *enemy, std::vector<Enemy*> &flock, 
    int enemyIndex, int flockIndex, Player &player, float deltaTime, bool swapOnNewIndex)
{
    enemy->update(player, deltaTime, flock);

    if (swapOnNewIndex && !AStar::singleton().isEntityOnIndex(*enemy, flockIndex))
    {
        int newIndex = AStar::singleton().getIndex(*enemy);
        std::swap(
            flock[enemyIndex],
            flock[flock.size() - 1]
        );
        flock.pop_back();

        m_enemies[newIndex == -1 ? 0 : newIndex].push_back(enemy);
    }
    else if (enemy->getHealth() <= 0)
    {
        m_aliveEnemies--;

        std::swap(
            flock[enemyIndex],
            flock[flock.size() - 1]
        );

        DeleteBody(*enemy);
        m_deadEnemies.push_back(enemy);
        flock.pop_back();
    }
}

void EntityManager::automaticUpdate(Player &player)
{
    // this is for automaticly finding crashes / bugs
    RandomGenerator gen = RandomGenerator::singleton();
    if (gen.getRandomInt(0, 1250) == 0)
    {
        player.getCharController()->warp(btVector3(
            gen.getRandomFloat(-250.f, 250.f), gen.getRandomFloat(5.f, 40.f), gen.getRandomFloat(-250.f, 250.f)
        ));
    }

    if (m_aliveEnemies < 100)
    {
        SpawnEnemy(EnemyType::NECROMANCER, btVector3(
            gen.getRandomFloat(-250.f, 250.f),
            gen.getRandomFloat(5.f, 40.f),
            gen.getRandomFloat(-250.f, 250.f)
        ), {});
    }

    if (gen.getRandomInt(0, 1900) == 0)
    {
        deallocateData(false);
        m_aliveEnemies = 0;
    }

    m_threadHandler->addWork(EnemyThreadHandler::WorkData{ this, gen.getRandomInt(0, m_enemies.size() - 1), &player });
}

void EntityManager::spawnWave(int waveId)
{
    if (m_enemies.empty())
    {
        printf("This will crash, data is not allocated, call allocateData() before spawning");
        return;
    }

    WaveManager::EntitiesInWave entities = m_waveManager.getEntities(waveId);
    m_frame = 0;

    btVector3 pos;
    RandomGenerator &generator = RandomGenerator::singleton();

    for (int entity : entities.enemies)
    {
        // just temp test values as of now, better with no random spawns?
        // should atleast check if spawn area is a walkable area
        // using nav mesh that would be easy but not trivial
        pos = { generator.getRandomFloat(-85, 85), generator.getRandomFloat(10, 25),
            generator.getRandomFloat(-85, 85) };

        SpawnEnemy(static_cast<EnemyType> (entity), pos, {});
    }

    for (WaveManager::Entity e : entities.triggers)
        SpawnTrigger(e.id, { e.x, e.y, e.z }, e.effects);

    for (WaveManager::Entity e : entities.bosses)
        SpawnEnemy(static_cast<EnemyType> (e.id), btVector3{ e.x, e.y, e.z }, e.effects);
}

Enemy* EntityManager::spawnEnemy(EnemyType id, btVector3 const &pos,
    std::vector<int> const &effects, Physics &physics, ProjectileManager *projectiles)
{
    if (m_aliveEnemies >= ENEMY_CAP) return nullptr;
    try
    {
        Enemy *enemy = m_enemyFactory[id](pos, 1.f, effects, physics);
        enemy->setSpawnFunctions(SpawnProjectile, SpawnEnemy, SpawnTrigger);

        int index = AStar::singleton().getIndex(*enemy);
        m_enemies[index == -1 ? 0 : index].push_back(enemy);
        m_aliveEnemies++;

        return enemy;
    }
    catch (std::exception& ex)
    {
        printf("Error when creating enemy, key: %d, reason: %s\n", static_cast<int> (id), ex.what());
    }
    return nullptr;
}

Trigger* EntityManager::spawnTrigger(int id, btVector3 const &pos,
    std::vector<int> &effects, Physics &physics, ProjectileManager *projectiles)
{
    // this is unefficient, could prolly be optimized but should only be done once per wave load
    std::vector<StatusManager::EFFECT_ID> effectsIds;
    effectsIds.reserve(effects.size());
    for (auto const &effect : effects)
        effectsIds.push_back(static_cast<StatusManager::EFFECT_ID> (effect));
    Trigger* trigger = m_triggerManager.addTrigger((Trigger::TriggerType)id, pos, physics, {}, effectsIds);

    return trigger;
}

size_t EntityManager::getNrOfAliveEnemies() const
{
    return m_aliveEnemies;
}

int EntityManager::giveEffectToAllEnemies(StatusManager::EFFECT_ID id)
{
    int i = 0;
    for (auto &vec : m_enemies)
        for (auto *enemy : vec)
        {
            enemy->getStatusManager().addStatus(id, 1, true);
            i++;
        }
    return i;
}

void EntityManager::render() const
{
    AStar &aStar = AStar::singleton();
    for (int i = 0; i < m_enemies.size(); ++i)
    {
        for (Enemy *enemy : m_enemies[i])
        {
            enemy->render();
            if (m_debugPath)  // should only be if _DEBUG is defined in the future
                enemy->debugRendering();
        }
    }

    for (int i = 0; i < m_deadEnemies.size(); ++i)
    {
     //   m_deadEnemies[i]->render(); // maybe add later
    }

    m_triggerManager.render();
    aStar.renderNavigationMesh();
}

const std::vector<std::vector<Enemy*>>& EntityManager::getAliveEnemies() const
{
    return m_enemies;
}

const WaveManager& EntityManager::getWaveManager() const
{
    return m_waveManager;
}

void EntityManager::setSpawnFunctions(ProjectileManager &projManager, Physics &physics)
{
    SpawnEnemy = [&](EnemyType type, btVector3 &pos, std::vector<int> effects) -> Enemy* {
        return spawnEnemy(type, pos, effects, physics, &projManager);
    };
    SpawnProjectile = [&](ProjectileData& pData, btVector3 position,
        btVector3 forward, Entity& shooter) -> Projectile* {
        return projManager.addProjectile(pData, position, forward, shooter);
    };
    SpawnTrigger = [&](int id, btVector3 const &pos, std::vector<int> &effects) -> Trigger* {
        return spawnTrigger(id, pos, effects, physics, &projManager);
    };

    // MOVE THESE TO SEPERATE FUNC
    DeleteBody = [&](Entity& entity) -> void {
        physics.removeRigidBody(entity.getRigidBody());
        for (int i = 0; i < entity.getNumberOfWeakPoints(); i++)
            physics.removeRigidBody(entity.getRigidBodyWeakPoint(i));
        entity.destroyBody();
    };
    AStar::singleton().generateNavigationMesh(physics);
    allocateData();
}