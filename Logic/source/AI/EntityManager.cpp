#include <AI/EntityManager.h>
using namespace Logic;

#define ENEMIES_PATH_UPDATE_PER_FRAME 200
#define ENEMIES_TEST_UPDATE_PER_FRAME 200
#define FILE_ABOUT_WHALES "Enemies/Wave"

#include <AI/EnemyTest.h>
#include <AI/EnemyNecromancer.h>
#include <AI/Behavior/AStar.h>
#include <DebugDefines.h>

#include <Engine\Profiler.h>
#include <Misc\RandomGenerator.h>

#include <ctime>
#include <stdio.h>

#define getThread(i) i % NR_OF_THREADS

EntityManager::EntityManager()
{
	m_currentWave = 0;
	m_frame = 0;

	allocateData();

	m_waveManager.setName(FILE_ABOUT_WHALES);
	m_waveManager.loadFile();

	resetThreads();
}

EntityManager::~EntityManager()
{
	deleteData();
	deleteThreads();
}

void EntityManager::allocateData()
{
	m_enemies.resize(AStar::singleton().getNrOfPolygons());
}

void EntityManager::resetThreads()
{
	for (std::thread *t : threads)
	{
		if (t)
		{
			t->join();
			delete t;
		}
		t = nullptr;
	}

	ZeroMemory(&m_threadRunning, sizeof(m_threadRunning));
}

void EntityManager::deleteThreads()
{
	for (std::thread *t : threads)
	{
		if (t)
		{
			deleteThread(t);
		}
	}
	ZeroMemory(&m_threadRunning, sizeof(m_threadRunning));
}

void EntityManager::joinAllThreads()
{
	for (auto *t : threads)
		t->join();
}

void EntityManager::deleteThread(std::thread *t)
{
	if (t->joinable())
		t->join();
	delete t;
}

void EntityManager::deleteData()
{
	for (std::vector<Enemy*> list : m_enemies)
		for (Enemy *enemy : list)
			delete enemy;
	for (Enemy *enemy : m_deadEnemies)
		delete enemy;
}

void EntityManager::update(Player const &player, float deltaTime) 
{
	m_frame++;
	m_deltaTime = deltaTime;
	
	PROFILE_BEGIN("EntityManager::update()");
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		if (m_enemies[i].size() > 0)
		{
			if ((i + m_frame) % ENEMIES_PATH_UPDATE_PER_FRAME != 0) {
				updateEnemies(i, player, deltaTime);
			}
			else
			{				// seperate threads based on i, but join them before end
				int thread = getThread(i);
				if (!m_threadRunning[thread])
				{
					m_threadRunning[thread] = true;
					std::thread *t = threads[thread];

					if (t)
					{
					//	deleteThread(t);
						m_indexRunning[thread] = i;
					}
					else
					{
						PROFILE_BEGIN("Create Thread");
						t = newd std::thread(EntityManager::onPathThreadCreation, this,
							i, std::ref(player), deltaTime);
						threads[thread] = t;
						PROFILE_END();
					}
				}
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
	Enemy *enemy;
	size_t size = m_enemies[index].size();
	
	for (size_t i = 0; i < size; ++i)
	{
		enemy = m_enemies[index][i];

		updateEnemy(enemy, index, player, deltaTime);

		if (i + m_frame % ENEMIES_TEST_UPDATE_PER_FRAME)
		{
			goalNodeChanged = enemy->getBehavior()->isGoalChangedAndSetToFalse();

			if (goalNodeChanged && !AStar::singleton().isEntityOnIndex(*enemy, index))
			{
				m_enemies[AStar::singleton().getIndex(*enemy)].push_back(enemy);
				std::swap(m_enemies[index][i], m_enemies[index][size - 1]);
				m_enemies[index].pop_back();
			}
		}
	}
}

void EntityManager::updateEnemiesAndPath(EntityManager *manager, int index, Player const &player, float deltaTime)
{
	Enemy *enemy;

	AStar &aStar = AStar::singleton();
	aStar.loadTargetIndex(player);

	std::vector<const DirectX::SimpleMath::Vector3*> path = aStar.getPath(index);
	auto &enemies = manager->m_enemies;

	for (size_t i = 0; i < enemies[index].size(); ++i)
	{
		enemy = enemies[index][i];
		manager->updateEnemy(enemy, index, player, deltaTime);

		enemy->getBehavior()->getPath().setPath(path); // TODO: enemy->setPath
	}

	manager->m_threadRunning[getThread(index)] = false;
	while (!manager->m_threadRunning[getThread(index)])
	{
		std::this_thread::sleep_for(10ms); // this is stupid but works
	}
	updateEnemiesAndPath(manager, manager->m_indexRunning[getThread(index)], player, manager->m_deltaTime);
}

void EntityManager::onPathThreadCreation(EntityManager * manager, int index, Player const & player, float deltaTime)
{
	g_Profiler->registerThread("Enemy Thread %d\0", getThread(index));
	updateEnemiesAndPath(manager, index, player, deltaTime);
}

void EntityManager::updateEnemy(Enemy *enemy, int index, Player const & player, float deltaTime)
{
	enemy->update(player, deltaTime, m_enemies[index]);

	if (enemy->getHealth() <= 0)
	{
		// Adds the score into the combo machine
		ComboMachine::Get().Kill(Enemy::ENEMY_TYPE(enemy->getEnemyType()));
		enemy->getRigidBody()->applyCentralForce({ 500.75f, 30000.f, 100.0f });

		std::swap(enemy, m_enemies[index][m_enemies[index].size() - 1]);
		m_deadEnemies.push_back(enemy);
		m_enemies[index].pop_back();
	}
}

void EntityManager::spawnWave(Physics &physics, ProjectileManager *projectiles) 
{
	if (m_enemies.empty())
	{
		printf("This will crash, data is not allocated, call allocateData() before spawning");
		return;
	}

	WaveManager::EntitiesInWave entities = m_waveManager.getEntities(m_currentWave);
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

		spawnEnemy(static_cast<Enemy::ENEMY_TYPE> (entity), pos, {}, physics, projectiles);
	}

	for (WaveManager::Entity e : entities.triggers)
	{
		spawnTrigger(e.id, { e.x, e.y, e.z }, e.effects, physics, projectiles);
	}

	for (WaveManager::Entity e : entities.bosses)
	{
		spawnEnemy(static_cast<Enemy::ENEMY_TYPE> (e.id), { e.x, e.y, e.z },
			e.effects, physics, projectiles);
	}
}

void EntityManager::spawnEnemy(Enemy::ENEMY_TYPE id, btVector3 const &pos,
	std::vector<int> const &effects, Physics &physics, ProjectileManager *projectiles)
{
	Enemy *enemy;
	int index;

	switch (id)
	{
		case Enemy::NECROMANCER:
			enemy = newd EnemyNecromancer(Graphics::ModelID::ENEMYGRUNT, physics.createBody(Logic::Cylinder({ pos }, { 0, 0, 0 }, {1.f, 2.f, 1.f}), 100, false), { 0.5f, 0.5f, 0.5f });
			break;
		default:
			enemy = newd EnemyTest(Graphics::ModelID::ENEMYGRUNT, physics.createBody(Sphere({ pos }, { 0, 0, 0 }, 1.f), 100, false), { 0.5f, 0.5f, 0.5f });
			break;
	}

	enemy->setEnemyType(id);
	enemy->addExtraBody(physics.createBody(Sphere({ 0, 0, 0 }, { 0, 0, 0 }, 1.f), 0.f, true), 2.f, { 0.f, 3.f, 0.f });
	enemy->setProjectileManager(projectiles);

#ifdef DEBUG_PATH
	enemy->getBehavior()->getPath().initDebugRendering(); // todo change to enemy->initDebugPath()
#endif

	index = AStar::singleton().getIndex(*enemy);
	if (index == -1)
		m_enemies[0].push_back(enemy);
	else
		m_enemies[index].push_back(enemy);
}

void EntityManager::spawnTrigger(int id, btVector3 const &pos,
	std::vector<int> &effects, Physics &physics, ProjectileManager *projectiles)
{
	// this is unefficient, could prolly be optimized but should only be done once per wave load
	std::vector<StatusManager::EFFECT_ID> effectsIds;
	effectsIds.reserve(effects.size());
	for (auto const &effect : effects)
		effectsIds.push_back(static_cast<StatusManager::EFFECT_ID> (effect));

	switch (id)
	{
		default:
			m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD,
				Cube(pos, { 0, 0, 0 }, { 2, 0.1f, 2 }),
				500.f, physics, { },
				effectsIds,
				true);
		break;
	}
}

size_t EntityManager::getEnemiesAlive() const 
{
    return m_enemies.size();
}

void EntityManager::clear()
{
	deleteData();

	m_deadEnemies.clear();
	m_enemies.clear();
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

void EntityManager::setCurrentWave(int currentWave) 
{
	m_currentWave = currentWave;
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
			m_deadEnemies[i]->render(renderer);
#endif
	}

	m_triggerManager.render(renderer);

#ifdef DEBUG_ASTAR
		AStar::singleton().renderNavigationMesh(renderer);
#endif
}

int EntityManager::getCurrentWave() const 
{
	return m_currentWave;
}