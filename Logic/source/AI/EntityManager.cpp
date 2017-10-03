#include <AI/EntityManager.h>
using namespace Logic;

#define ENEMY_START_COUNT 16
#define ENEMIES_PATH_UPDATE_PER_FRAME 3
#define TEST_NAME "helloWave"
#define DEBUG_ASTAR true
#define DEBUG_PATH true

#include <AI/EnemyTest.h>
#include <AI/EnemyNecromancer.h>

#include <AI\Behavior\AStar.h>
#include <Engine\Profiler.h>
#include <ctime>
#include <stdio.h>

EntityManager::EntityManager()
{
	m_currentWave = 0;
	m_frame = 0;

	reserveData();

	m_waveManager.setName(TEST_NAME);
}


EntityManager::~EntityManager()
{
	for (Enemy *enemy : m_enemies)
		delete enemy;
	for (Enemy *enemy : m_bossEnemies)
		delete enemy;
	for (Enemy *enemy : m_deadEnemies)
		delete enemy;
}

void EntityManager::reserveData()
{
	m_bossEnemies.reserve(ENEMY_START_COUNT);
	m_enemies.reserve(ENEMY_START_COUNT);
	m_deadEnemies.reserve(ENEMY_START_COUNT);
}

void EntityManager::update(Player const &player, float deltaTime) 
{
	clock_t begin = clock();
	m_frame++;
	PROFILE_BEGIN("EntityManager::update()");
	
	AStar::singleton().loadTargetIndex(player);
	for (int i = 0; i < m_enemies.size(); ++i)
	{
		m_enemies[i]->update(player, deltaTime, (i + m_frame) % ENEMIES_PATH_UPDATE_PER_FRAME == 0);
		if (m_enemies[i]->getHealth() <= 0) {
			m_deadEnemies.push_back(m_enemies[i]);
			std::swap(m_enemies[i], m_enemies[m_enemies.size() - 1]);
			m_enemies.pop_back();
		}
	}

	for (int i = 0; i < m_bossEnemies.size(); ++i)
	{
		m_bossEnemies[i]->update(player, deltaTime);
	}

	for (int i = 0; i < m_deadEnemies.size(); ++i)
	{
		m_deadEnemies[i]->updateDead(deltaTime);
	}

		
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	printf("Entity Time Elapsed: %f seconds, (EntityManager.cpp:%d)\n", elapsed_secs, __LINE__);

	m_triggerManager.update(deltaTime);
	PROFILE_END();
}

void EntityManager::spawnWave(Physics &physics) 
{
	std::vector<int> enemies = m_waveManager.getEnemies(m_currentWave);
	m_enemies.reserve(enemies.size() + m_enemies.size());
	m_frame = 0;

	if (m_currentWave == 1)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			i += 1;
			//m_enemies.push_back(new EnemyTest(physics.createBody(Cube({ i * 8.f, i * 10.f, i * 1.f }, { 0, 0, 0 }, { 0.5f, 0.5f, 0.5f}), 100, false), { 0.5f, 0.5f, 0.5f}));
            m_enemies.push_back(new EnemyNecromancer(Graphics::ModelID::CUBE, physics.createBody(Sphere({ 0, 0, 0 }, { 0, 0, 0 }, 0.5f), 100, false), { 0.5f, 0.5f, 0.5f }));
		}
		/*
		m_triggerManager.addTrigger(Cube({ 10, 0.1f, 10 }, { 0, 0, 0 }, { 2, 0.1f, 2 }), 500.f, physics, { StatusManager::UPGRADE_ID::BOUNCE }, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ -10, 0.1f, 10 }, { 0, 0, 0 }, { 2, 0.1f, 2 }), 500.f, physics, { StatusManager::UPGRADE_ID::BOUNCE }, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ -10, 0.1f, -10 }, { 0, 0, 0 }, { 2, 0.1f, 2 }), 500.f, physics, { StatusManager::UPGRADE_ID::BOUNCE }, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 10, 0.1f, -10 }, { 0, 0, 0 }, { 2, 0.1f, 2 }), 500.f, physics, { StatusManager::UPGRADE_ID::BOUNCE }, { StatusManager::EFFECT_ID::BOOST_UP }, true);

		m_triggerManager.addTrigger(Cube({ 15, 10.f, 5 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 20, 15.f, 10 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 25, 18.f, -5 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 30, 25.f, -0 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 45, 30.f, 12 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 50, 40.f, -5 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 40, 30.f, 2 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 70, 54.f, 10 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 80, 80.f, -2 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP, StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 50, 65.f, 5 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Cube({ 130, 128.f, 5 }, { 0, 0, 0 }, { 10, 1.f, 10 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP, StatusManager::EFFECT_ID::BOOST_UP, StatusManager::EFFECT_ID::BOOST_UP, StatusManager::EFFECT_ID::BOOST_UP });

		// Ammo refiller
		m_triggerManager.addTrigger(Cube({ 0, 10, 40 }, { 0, 0, 0 }, { 10, 10, 10 }), 1000.f, physics, { }, { StatusManager::EFFECT_ID::AMMO_PICK_UP });
		*/
	}
}

int EntityManager::getEnemiesAlive() const 
{
    return m_deadEnemies.size();
}

void EntityManager::clear() 
{
	m_deadEnemies.clear();
	m_enemies.clear();
	m_bossEnemies.clear();

	reserveData();
}

void EntityManager::setCurrentWave(int currentWave) 
{
	m_currentWave = currentWave;
}

void EntityManager::render(Graphics::Renderer &renderer)
{
	for (int i = 0; i < m_enemies.size(); ++i)
	{
		m_enemies[i]->render(renderer);
		if (DEBUG_PATH)
			m_enemies[i]->debugRendering(renderer);
	}

	for (int i = 0; i < m_bossEnemies.size(); ++i)
	{
		m_bossEnemies[i]->render(renderer);
	}

	for (int i = 0; i < m_deadEnemies.size(); ++i)
	{
		m_deadEnemies[i]->render(renderer);
	}

	m_triggerManager.render(renderer);

	if (DEBUG_ASTAR)
		AStar::singleton().renderNavigationMesh(renderer);
}

int EntityManager::getCurrentWave() const 
{
	return m_currentWave;
}