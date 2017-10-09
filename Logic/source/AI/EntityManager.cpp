#include <AI/EntityManager.h>
using namespace Logic;

#define ENEMIES_PATH_UPDATE_PER_FRAME 3
#define TEST_NAME "helloWave"
#define DEBUG_ASTAR false
#define DEBUG_PATH false

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

	allocateData();

	m_waveManager.setName(TEST_NAME);
}


EntityManager::~EntityManager()
{
	deleteData();
}

void EntityManager::allocateData()
{
	m_enemies.resize(AStar::singleton().getNrOfPolygons());
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
//	PROFILE_BEGIN("EntityManager::update()");
	
	AStar::singleton().loadTargetIndex(player);

	for (int i = 0; i < m_enemies.size(); i++)
	{
		updateEnemies(i, player, deltaTime,
			(i + m_frame) % ENEMIES_PATH_UPDATE_PER_FRAME == 0);
	}

	for (int i = 0; i < m_deadEnemies.size(); ++i)
	{
		m_deadEnemies[i]->updateDead(deltaTime);
	}

	m_triggerManager.update(deltaTime);

//	PROFILE_END();
}

void EntityManager::updateEnemies(int index, Player const &player, float deltaTime,
	bool updatePath)
{
	Enemy *enemy;
	for (int i = 0; i < m_enemies[index].size(); ++i)
	{
		enemy = m_enemies[index][i];
		enemy->update(player, deltaTime, m_enemies[index], updatePath);

		if (enemy->getHealth() <= 0) {
			m_deadEnemies.push_back(enemy);
			std::swap(enemy, m_enemies[index][m_enemies[index].size() - 1]);
			m_enemies[index].pop_back();
		}
	}
}

void EntityManager::spawnWave(Physics &physics, ProjectileManager *projectiles) 
{
	if (m_enemies.empty())
	{
		printf("This will crash, data is not allocated, call allocateData() before spawning");
		return;
	}

	std::vector<int> enemies = m_waveManager.getEnemies(m_currentWave);
	m_enemies.reserve(enemies.size() + m_enemies.size());
	m_frame = 0;

	AStar &aStar = AStar::singleton();
	int index;

	if (m_currentWave == 1)
	{
		Enemy *enemy;
		for (int i = 0; i < enemies.size(); i++)
		{
			//m_enemies.push_back(new EnemyTest(physics.createBody(Cube({ i * 8.f, i * 10.f, i * 1.f }, { 0, 0, 0 }, { 0.5f, 0.5f, 0.5f}), 100, false), { 0.5f, 0.5f, 0.5f}));
			enemy = newd EnemyNecromancer(Graphics::ModelID::ENEMYGRUNT, physics.createBody(Sphere({ i * 5.f, 0, i * 5.f }, { 0, 0, 0 }, 1.f), 100, false), { 0.5f, 0.5f, 0.5f });
			enemy->addExtraBody(physics.createBody(Sphere({ 0, 0, 0 }, { 0, 0, 0 }, 1.f), 0.f, true), 2.f, {0.f, 3.f, 0.f});
			enemy->setProjectileManager(projectiles);

			index = aStar.getIndex(*enemy);
			if (index == -1)
				m_enemies[0].push_back(enemy); //TODO
			else
				m_enemies[index].push_back(enemy);
		}
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 10, 0.1f, 10 }, { 0, 0, 0 }, { 2, 0.1f, 2 }), 500.f, physics, { StatusManager::UPGRADE_ID::BOUNCE }, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ -10, 0.1f, 10 }, { 0, 0, 0 }, { 2, 0.1f, 2 }), 500.f, physics, { StatusManager::UPGRADE_ID::BOUNCE }, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ -10, 0.1f, -10 }, { 0, 0, 0 }, { 2, 0.1f, 2 }), 500.f, physics, { StatusManager::UPGRADE_ID::BOUNCE }, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 10, 0.1f, -10 }, { 0, 0, 0 }, { 2, 0.1f, 2 }), 500.f, physics, { StatusManager::UPGRADE_ID::BOUNCE }, { StatusManager::EFFECT_ID::BOOST_UP }, true);

		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 15, 10.f, 5 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 20, 15.f, 10 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 25, 18.f, -5 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 30, 25.f, -0 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 45, 30.f, 12 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 50, 40.f, -5 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 40, 30.f, 2 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 70, 54.f, 10 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 80, 80.f, -2 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP, StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 50, 65.f, 5 }, { 0, 0, 0 }, { 2, 1.f, 2 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP }, true);
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 130, 128.f, 5 }, { 0, 0, 0 }, { 10, 1.f, 10 }), 500.f, physics, {}, { StatusManager::EFFECT_ID::BOOST_UP, StatusManager::EFFECT_ID::BOOST_UP, StatusManager::EFFECT_ID::BOOST_UP, StatusManager::EFFECT_ID::BOOST_UP });

		// Ammo refiller
		m_triggerManager.addTrigger(Graphics::ModelID::JUMPPAD, Cube({ 0, 10, 40 }, { 0, 0, 0 }, { 10, 10, 10 }), 1000.f, physics, { }, { StatusManager::EFFECT_ID::AMMO_PICK_UP });
	}
}

int EntityManager::getEnemiesAlive() const 
{
    return m_enemies.size();
}

void EntityManager::clear() 
{
	deleteData();

	m_deadEnemies.clear();
	m_enemies.clear();
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
			if (DEBUG_PATH)
				enemy->debugRendering(renderer);
		}
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