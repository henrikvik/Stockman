#include <AI/EntityManager.h>
using namespace Logic;

#define ENEMY_START_COUNT 64
#define TEST_NAME "helloWave.xml"

EntityManager::EntityManager()
{
	m_currentWave = 0;

	m_enemiesDeadBool.reserve(ENEMY_START_COUNT);
	m_enemies.reserve(ENEMY_START_COUNT);

	m_waveManager.setName(TEST_NAME);
}


EntityManager::~EntityManager()
{
	for (Enemy *enemy : m_enemies)
		delete enemy;
}

void EntityManager::update(float deltaTime) 
{
	for (int i = 0; i < m_enemiesDeadBool.size(); i++)
	{
		if (m_enemiesDeadBool[i])
		{
			m_enemies[i]->update(deltaTime);
		}
	}
}

void EntityManager::spawnWave() 
{
	std::vector<int> enemies = m_waveManager.getEnemies(m_currentWave++);
	m_enemies.reserve(enemies.size() + m_enemies.size());
	m_enemiesDeadBool.reserve(enemies.size() + m_enemiesDeadBool.size());

	for (int i = 0; i < enemies.size(); i++) 
	{
		// m_enemies.push_back(new Enemy()); TODO
		m_enemiesDeadBool.push_back(false);
	}
}

int EntityManager::getEnemiesAlive() const 
{
    return m_enemiesDeadBool.size();
}

void EntityManager::clear() 
{
	m_enemiesDeadBool.clear();
	m_enemies.clear();

	m_enemiesDeadBool.reserve(ENEMY_START_COUNT);
	m_enemies.reserve(ENEMY_START_COUNT);
}

void EntityManager::setCurrentWave(int currentWave) 
{
	m_currentWave = currentWave;
}

int EntityManager::getCurrentWave() const 
{
	return m_currentWave;
}