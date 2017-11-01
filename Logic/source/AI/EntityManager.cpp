#include <AI/EntityManager.h>
using namespace Logic;

#define ENEMIES_PATH_UPDATE_PER_FRAME 25
#define ENEMIES_TEST_UPDATE_PER_FRAME 25 
#define FILE_ABOUT_WHALES "Enemies/Wave"

#include <AI\Behavior\EnemyThreadHandler.h>
#include <AI\Behavior\AStar.h>
#include <AI\EnemyTest.h>
#include <AI\EnemyNecromancer.h>
#include <AI\EnemyChaser.h>
#include <Misc\ComboMachine.h>

#include <Player\Player.h>
#include <Projectile\ProjectileManager.h>

#include <Graphics\include\Renderer.h>
#include <Graphics\include\Structs.h>
#include <Physics\Physics.h>
#include <Graphics\include\Particles\ParticleSystem.h>

#include <DebugDefines.h>
#include <Misc\RandomGenerator.h>

#include <Engine\DebugWindow.h>
#include <Engine\Profiler.h>

#include <ctime>
#include <stdio.h>

const int EntityManager::NR_OF_THREADS = 4;

EntityManager::EntityManager()
{
    m_frame = 0;
    m_aliveEnemies = 0;
    m_aiType = NORMAL_MODE;

    allocateData();
    loadDebugCmds();

    m_waveManager.setName(FILE_ABOUT_WHALES);
    m_waveManager.loadFile();
}

EntityManager::~EntityManager()
{
    deallocateData();
    delete m_threadHandler;
}

void EntityManager::allocateData()
{
    m_enemies.resize(AStar::singleton().getNrOfPolygons());
    m_threadHandler = newd EnemyThreadHandler();
}

void EntityManager::loadDebugCmds()
{
#ifdef _DEBUG
    DebugWindow::getInstance()->registerCommand("SETAI", [&](std::vector<std::string> &para) -> std::string {
        try {
            m_aiType = static_cast<AIType> (stoi(para[0]));
            return "AI Mode Updated";
        } catch (std::exception e) {
            return "No, Chaos is a ladder.";
        }
    });
#endif
}

void EntityManager::deallocateData(bool forceDestroy)
{
    m_threadHandler->deleteThreads();
    for (std::vector<Enemy*>& list : m_enemies)
    {
        for (Enemy *&enemy : list)
        {
            if (forceDestroy || !enemy->hasCallbackEntities())
            {
                DeleteBody(*enemy);
                delete enemy;
                enemy = nullptr;
            }
            else {
                m_deadEnemies.push_back(enemy);
            }
        }
        list.clear();
    }

    // clear out enemies without callbacks in needs to do
    for (Enemy *&enemy : m_deadEnemies)
    {
        if (forceDestroy || !enemy->hasCallbackEntities()) 
        {
            DeleteBody(*enemy);
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
    m_threadHandler->initThreads();
}

void EntityManager::update(Player const &player, float deltaTime)
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
                if ((i + m_frame) % ENEMIES_PATH_UPDATE_PER_FRAME == 0)
                    m_threadHandler->addWork({ this, static_cast<int> (i), &player });
            }
        }
    }
	PROFILE_END();

	for (int i = 0; i < m_deadEnemies.size(); ++i)
	{
		m_deadEnemies[i]->updateDead(deltaTime);
	}

	m_triggerManager.update(deltaTime);
}

void EntityManager::updateEnemies(int index, Player const &player, float deltaTime)
{
	bool goalNodeChanged = false;
    std::vector<Enemy*> &enemies = m_enemies[index];
	
	for (size_t i = 0; i < enemies.size(); ++i)
	{
        updateEnemy(enemies[i], enemies, (int)i, index, player, deltaTime, true);
	}
}

void EntityManager::updateEnemy(Enemy *enemy, std::vector<Enemy*> &flock, 
    int enemyIndex, int flockIndex, Player const &player, float deltaTime, bool swapOnNewIndex)
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

        m_deadEnemies.push_back(enemy);
        flock.pop_back();
    }
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

        SpawnEnemy(static_cast<ENEMY_TYPE> (entity), pos, {});
    }

    for (WaveManager::Entity e : entities.triggers)
        SpawnTrigger(e.id, { e.x, e.y, e.z }, e.effects);

    for (WaveManager::Entity e : entities.bosses)
        SpawnEnemy(static_cast<ENEMY_TYPE> (e.id), btVector3{ e.x, e.y, e.z }, e.effects);
}

Enemy* EntityManager::spawnEnemy(ENEMY_TYPE id, btVector3 const &pos,
    std::vector<int> const &effects, Physics &physics, ProjectileManager *projectiles)
{
    Enemy *enemy;
    int index;
    btRigidBody *testBody = physics.createBody(Cube({ pos }, { 0, 0, 0 }, { 1.f, 1.f, 1.f }), 100, false, Physics::COL_ENEMY, (Physics::COL_EVERYTHING /*&~Physics::COL_PLAYER*/));
    
    // Restrict "tilting" over
    testBody->setAngularFactor(btVector3(0, 1, 0));

    switch (id)
    {
    case ENEMY_TYPE::NECROMANCER:
        enemy = newd EnemyNecromancer(Graphics::ModelID::ENEMYGRUNT, testBody, { 0.5f, 0.5f, 0.5f });
        break;
    case ENEMY_TYPE::NECROMANCER_MINION:
        enemy = newd EnemyChaser(testBody);
        break;
    default:
        enemy = newd EnemyTest(Graphics::ModelID::ENEMYGRUNT, testBody, { 0.5f, 0.5f, 0.5f });
		break;
    }

    enemy->setEnemyType(id);
    enemy->addExtraBody(physics.createBody(Cube({ 0, 0, 0 }, { 0, 0, 0 }, { 1.f, 1.f, 1.f }), 0.f, true, Physics::COL_ENEMY, (Physics::COL_EVERYTHING /*&~Physics::COL_PLAYER*/)), 2.f, { 0.f, 3.f, 0.f });

    enemy->setSpawnFunctions(SpawnProjectile, SpawnEnemy, SpawnTrigger);

#ifdef DEBUG_PATH
    enemy->getBehavior()->getPath().initDebugRendering(); // todo change to enemy->initDebugPath()
#endif

    index = AStar::singleton().getIndex(*enemy);
    m_enemies[index == -1 ? 0 : index].push_back(enemy);
    m_aliveEnemies++;

    return enemy;
}

Trigger* EntityManager::spawnTrigger(int id, btVector3 const &pos,
    std::vector<int> &effects, Physics &physics, ProjectileManager *projectiles)
{
    // this is unefficient, could prolly be optimized but should only be done once per wave load
    std::vector<StatusManager::EFFECT_ID> effectsIds;
    effectsIds.reserve(effects.size());
    for (auto const &effect : effects)
        effectsIds.push_back(static_cast<StatusManager::EFFECT_ID> (effect));
    Trigger *trigger;

    switch (id)
    {
    case 1: // wtf, starts at 1
        trigger = m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD,
            Cube(pos, { 0, 0, 0 }, { 2, 0.1f, 2 }),
            500.f, physics, {},
            effectsIds,
            true);
        break;
    case 2:
        trigger = m_triggerManager.addTrigger(Graphics::ModelID::AMMOBOX,
            Cube(pos, { 0, 0, 0 }, { 1, 0.5, 1 }), 0.f, physics, {},
            effectsIds,
            false);
        break;
    default:
        trigger = nullptr;
    }

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

void EntityManager::render(Graphics::Renderer &renderer)
{
    for (int i = 0; i < m_enemies.size(); ++i)
    {
        for (Enemy *enemy : m_enemies[i])
        {
            enemy->render(renderer);
#ifdef DEBUG_PATH	
            enemy->debugRendering(renderer);
#endif
        }
    }

    for (int i = 0; i < m_deadEnemies.size(); ++i)
    {
#ifndef DISABLE_RENDERING_DEAD_ENEMIES
     //   m_deadEnemies[i]->render(renderer);
#endif
    }

    m_triggerManager.render(renderer);

#ifdef DEBUG_ASTAR
    AStar::singleton().renderNavigationMesh(renderer);
#endif
}

const std::vector<std::vector<Enemy*>>& EntityManager::getAliveEnemies() const
{
    return m_enemies;
}

const WaveManager& Logic::EntityManager::getWaveManager() const
{
    return m_waveManager;
}

void EntityManager::setSpawnFunctions(ProjectileManager &projManager, Physics &physics)
{
    SpawnEnemy = [&](ENEMY_TYPE type, btVector3 &pos, std::vector<int> effects) -> Enemy* {
        return spawnEnemy(type, pos, effects, physics, &projManager);
    };
    SpawnProjectile = [&](ProjectileData& pData, btVector3 position,
        btVector3 forward, Entity& shooter) -> Projectile* {
        return projManager.addProjectile(pData, position, forward, shooter);
    };
    SpawnTrigger = [&](int id, btVector3 const &pos, std::vector<int> &effects) -> Trigger* {
        return spawnTrigger(id, pos, effects, physics, &projManager);
    };
    DeleteBody = [&](Entity& entity) -> void {
        physics.removeRigidBody(entity.getRigidBody());
        for (int i = 0; i < entity.getNumberOfWeakPoints(); i++)
            physics.removeRigidBody(entity.getRigidBodyWeakPoint(i));
        entity.destroyBody();
    };
}