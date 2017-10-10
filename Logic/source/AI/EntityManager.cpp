#include <AI/EntityManager.h>
using namespace Logic;

#define ENEMIES_PATH_UPDATE_PER_FRAME 20
#define FILE_ABOUT_WHALES "Enemies/Wave"
#define DEBUG_ASTAR false
#define DEBUG_PATH false

#include <AI/EnemyTest.h>
#include <AI/EnemyNecromancer.h>
#include <AI/Behavior/AStar.h>

#include <Engine\Profiler.h>
#include <Misc\RandomGenerator.h>

#include <ctime>
#include <stdio.h>

EntityManager::EntityManager()
{
	m_currentWave = 0;
	m_frame = 0;

	allocateData();

	m_waveManager.setName(FILE_ABOUT_WHALES);
	m_waveManager.loadFile();
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
}

void EntityManager::updateEnemies(int index, Player const &player, float deltaTime,
	bool updatePath)
{
	Enemy *enemy;
	int newIndex;
	for (int i = 0; i < m_enemies[index].size(); ++i)
	{
		enemy = m_enemies[index][i];
		enemy->update(player, deltaTime, m_enemies[index], updatePath);

		if (updatePath)
		{
			newIndex = AStar::singleton().getIndex(*enemy);
			if (newIndex != -1 && newIndex != index)
			{
				m_enemies[newIndex].push_back(enemy);
				std::swap(m_enemies[index][i], 
					m_enemies[index][m_enemies[index].size() - 1]);
				m_enemies[index].pop_back();
			}
		}

		if (enemy->getHealth() <= 0) {
			enemy->getRigidBody()->applyCentralForce({ 500.75f, 30000.f, 100.0f}); // FOR TESTING, death animation should play instead
			m_deadEnemies.push_back(enemy);
			std::swap(m_enemies[index][i],
				m_enemies[index][m_enemies[index].size() - 1]);
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

	WaveManager::EntitiesInWave entities = m_waveManager.getEntities(m_currentWave);
	m_frame = 0;

	int index;

	Enemy *enemy;
	btVector3 pos;
	RandomGenerator &generator = RandomGenerator::singleton();

	for (int entity : entities.enemies)
	{
		// just temp test values as of now, better with no random spawns?
		// should atleast check if spawn area is a walkable area
		// using nav mesh that would be easy but not trivial
		pos = { generator.getRandomFloat(0, 100), generator.getRandomFloat(10, 15),
				generator.getRandomFloat(0, 100) };

		spawnEnemy(static_cast<EnemyType> (entity), pos, {}, physics, projectiles);
	}

	for (WaveManager::Entity e : entities.triggers)
	{
		spawnTrigger(e.id, { e.x, e.y, e.z }, e.effects, physics, projectiles);
	}

	for (WaveManager::Entity e : entities.bosses)
	{
		spawnEnemy(static_cast<EnemyType> (e.id), { e.x, e.y, e.z },
			e.effects, physics, projectiles);
	}
}

void EntityManager::spawnEnemy(EnemyType id, btVector3 const &pos,
	std::vector<int> const &effects, Physics &physics, ProjectileManager *projectiles)
{
	Enemy *enemy;
	int index;

	switch (id)
	{
		case NECROMANCER:
			enemy = newd EnemyNecromancer(Graphics::ModelID::ENEMYGRUNT, physics.createBody(Sphere({ pos }, { 0, 0, 0 }, 1.f), 100, false), { 0.5f, 0.5f, 0.5f });
			break;
		default:
			enemy = newd EnemyTest(Graphics::ModelID::ENEMYGRUNT, physics.createBody(Sphere({ pos }, { 0, 0, 0 }, 1.f), 100, false), { 0.5f, 0.5f, 0.5f });
			break;
	}

	enemy->addExtraBody(physics.createBody(Sphere({ 0, 0, 0 }, { 0, 0, 0 }, 1.f), 0.f, true), 2.f, { 0.f, 3.f, 0.f });
	enemy->setProjectileManager(projectiles);

	index = AStar::singleton().getIndex(*enemy);
	if (index == -1)
		m_enemies[0].push_back(enemy);
	else
		m_enemies[index].push_back(enemy);
}

void EntityManager::spawnTrigger(int id, btVector3 const &pos,
	std::vector<int> &effects, Physics &physics, ProjectileManager *projectiles)
{
	// this is unefficient, could prolly be optimized but should be done once per wave load
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