#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <thread>

#include <AI/WaveManager.h>
#include <AI/TriggerManager.h>

#pragma region ClassDesc
	/*
		CLASS: EntityManager (Bad name TODO: change)
		AUTHOR: Lukas Westling

		This class handles Enemies and how it spawns
		with the help of WaveManager.

		It also handles triggers and how it spawns. (WaveManager?)
	*/
#pragma endregion

namespace Logic 
{
    class ProjectileManager;
    class Physics;
    class Enemy;
    class Renderer;
    class EnemyThreadHandler;

	class EntityManager
	{
	private:
		static const int NR_OF_THREADS = 4;

		std::vector<std::vector<Enemy*>> m_enemies;
		std::vector<Enemy*> m_deadEnemies;
		std::vector<double> time;

		TriggerManager m_triggerManager;
		WaveManager m_waveManager;
        EnemyThreadHandler *m_threadHandler; // Just because i want to delete it when i want

		int m_currentWave, m_frame;
		float m_deltaTime;

		void deleteData(); // delete data in vectors
		void allocateData(); // resize enemy vector 

        std::function<Projectile*(ProjectileData& pData, btVector3 position,
            btVector3 forward, Entity& shooter)> SpawnProjectile;
        std::function<Enemy*(btVector3 &pos,ENEMY_TYPE type)> SpawnEnemy;
        std::function<Trigger*(int id, btVector3 const &pos,
            std::vector<int> &effects)> SpawnTrigger;
	public:
		EntityManager();
		EntityManager(EntityManager const &entityManager) = delete;
		~EntityManager();

		void update(Player const &player, float deltaTime);
		void updateEnemies(int index, Player const &player, float deltaTime);
		// statis because threads will use this
		void updateEnemy(Enemy *enemy, int index, Player const &player, float deltaTime);
		void clear();

		int giveEffectToAllEnemies(StatusManager::EFFECT_ID id);

		void spawnWave(Physics &physics, ProjectileManager *projectiles);
		Enemy* spawnEnemy(ENEMY_TYPE id, btVector3 const &pos, std::vector<int> const &effects,
			Physics &physics, ProjectileManager *projectiles);
		Trigger* spawnTrigger(int id, btVector3 const &pos, std::vector<int> &effects,
			Physics &physics, ProjectileManager *projectiles);

		void setCurrentWave(int currentWave);
		void render(Graphics::Renderer &renderer);

		size_t getNrOfAliveEnemies() const;
		int getCurrentWave() const;
        const std::vector<std::vector<Enemy*>>& getAliveEnemies() const;

		EntityManager* operator=(EntityManager const &entityManager) = delete;
        void setSpawnFunctions(ProjectileManager &projManager, Physics &physics);
	};
}

#endif