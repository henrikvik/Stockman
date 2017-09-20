#include <AI/EntityManager.h>
using namespace Logic;

#define ENEMY_START_COUNT 16
#define TEST_NAME "helloWave.xml"

#include <AI/EnemyTest.h>
#include <ctime>
#include <stdio.h>

EntityManager::EntityManager()
{
	m_currentWave = 0;

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

void EntityManager::update(float deltaTime) 
{
	clock_t begin = clock();

	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->update(deltaTime);
	}

	for (int i = 0; i < m_bossEnemies.size(); i++)
	{
		m_bossEnemies[i]->update(deltaTime);
	}

	for (int i = 0; i < m_deadEnemies.size(); i++)
	{
		m_deadEnemies[i]->updateDead(deltaTime);
	}

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//	printf("t: %f\n", elapsed_secs);
}

void EntityManager::spawnWave() 
{
	std::vector<int> enemies = m_waveManager.getEnemies(m_currentWave++);
	m_enemies.reserve(enemies.size() + m_enemies.size());

	for (int i = 0; i < enemies.size(); i++)
	{
		m_enemies.push_back(new EnemyTest(nullptr, {0, 0, 0})); //alter this
		m_deadEnemies.push_back(new EnemyTest(nullptr, { 0, 0, 0 })); //alter this
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

}

int EntityManager::getCurrentWave() const 
{
	return m_currentWave;
}