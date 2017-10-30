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
		static const int NR_OF_THREADS;

		std::vector<std::vector<Enemy*>> m_enemies;
		std::vector<Enemy*> m_deadEnemies;
		std::vector<double> time;

		TriggerManager m_triggerManager;
		WaveManager m_waveManager;
        EnemyThreadHandler *m_threadHandler; // Just because i want to delete it when i want

		int m_frame;
		float m_deltaTime;

		void deleteData(); // delete data in vectors
		void allocateData(); // resize enemy vector 

        std::function<Projectile*(ProjectileData& pData, btVector3 position,
            btVector3 forward, Entity& shooter)>            SpawnProjectile;
        std::function<Enemy*(ENEMY_TYPE type, btVector3 &pos,
            std::vector<int> effects)>                      SpawnEnemy;
        std::function<Trigger*(int id, btVector3 const &pos,
            std::vector<int> &effects)>                     SpawnTrigger;
	public:
		EntityManager();
		EntityManager(EntityManager const &entityManager) = delete;
		EntityManager* operator=(EntityManager const &entityManager) = delete;
		~EntityManager();

        // data handling
		void clear();

        // render / updates
		void render(Graphics::Renderer &renderer);

		void update(Player const &player, float deltaTime);
		void updateEnemies(int index, Player const &player, float deltaTime);
		// statis because threads will use this
		void updateEnemy(Enemy *enemy, std::vector<Enemy*> &flock, int enemyIndex,
            int flockIndex, Player const &player, float deltaTime, bool swapOnNewIndex);

        // effects
		int giveEffectToAllEnemies(StatusManager::EFFECT_ID id);

        // spawning
		void spawnWave(int waveId);
		Enemy* spawnEnemy(ENEMY_TYPE id, btVector3 const &pos, std::vector<int> const &effects,
			Physics &physics, ProjectileManager *projectiles);
		Trigger* spawnTrigger(int id, btVector3 const &pos, std::vector<int> &effects,
			Physics &physics, ProjectileManager *projectiles);

        // sets & gets

        void setSpawnFunctions(ProjectileManager &projManager, Physics &physics);

		size_t getNrOfAliveEnemies() const;

        const WaveManager& getWaveManager() const;
        const std::vector<std::vector<Enemy*>>& getAliveEnemies() const;
	};
}

#endif