#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <thread>

#include <AI/WaveManager.h>
#include <AI/Trigger/TriggerManager.h>

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
    enum TriggerType;

	class EntityManager
	{
    typedef std::function<Enemy* (btVector3 const &pos, float scale,
        std::vector<int> const &effects, Physics &physics)> EnemyFactoryFunc;

    public:
        enum AIType {
            NORMAL_MODE, NO_AI_MODE, HARDCORE, DARK_SOULS
        };
	private:
		static const int NR_OF_THREADS, ENEMY_CAP;
        static int       PATH_UPDATE_DIV;
        static const btVector3 MIN_SPAWN, MAX_SPAWN; // position of min / max like a cube
        static const float     INVALID_LENGTH;

		std::vector<std::vector<Enemy*>> m_enemies;
		std::vector<Enemy*> m_deadEnemies;
        std::unordered_map<EnemyType, EnemyFactoryFunc> m_enemyFactory;

		TriggerManager m_triggerManager;
		WaveManager m_waveManager;
        EnemyThreadHandler *m_threadHandler; // Just because i want to delete it when i want

        int m_frame, m_aliveEnemies;
        bool m_automaticTesting, m_debugPath;

        AIType m_aiType;
		float m_deltaTime;

		void allocateData(); // resize enemy vector
        void loadDebugCmds();

        std::function<void(Entity& entity)>                 DeleteBody;
        std::function<Projectile*(ProjectileData& pData, btVector3 position,
            btVector3 forward, Entity& shooter)>            SpawnProjectile;
        std::function<Enemy*(EnemyType type, btVector3 &pos,
            std::vector<int> effects)>                      SpawnEnemy;
        std::function<Trigger*(int id, btVector3 const &pos,
            std::vector<int> &effects)>                     SpawnTrigger;
	public:
		EntityManager();
		EntityManager(EntityManager const &entityManager) = delete;
		EntityManager* operator=(EntityManager const &entityManager) = delete;
		~EntityManager();

        void registerCreationFunctions();

        // data handling
        void resetTriggers();
        void deallocateData(bool forceDestroy = true); // delete data in vectors

        // render / updates
		void render() const;

		void update(Player &player, float deltaTime);
		void updateEnemies(int index, Player &player, float deltaTime);
		void updateEnemy(Enemy *enemy, std::vector<Enemy*> &flock, int enemyIndex,
            int flockIndex, Player &player, float deltaTime, bool swapOnNewIndex);
        void automaticUpdate(Player &player);

        // effects
		int giveEffectToAllEnemies(StatusManager::EFFECT_ID id);

        // spawning
		void spawnWave(int waveId, btVector3 const &playerPos);
		Enemy* spawnEnemy(EnemyType id, btVector3 const &pos, std::vector<int> const &effects,
			Physics &physics, ProjectileManager *projectiles);
		Trigger* spawnTrigger(int id, btVector3 const &pos,
            std::vector<int> &effects, Physics &physics, ProjectileManager *projectiles);
        // a circle of area enemies can't spawn
        btVector3 getRandomSpawnLocation(btVector3 const &invalidPoint, float invalidLength);

        // sets & gets
        void setSpawnFunctions(ProjectileManager &projManager, Physics &physics);

		size_t getNrOfAliveEnemies() const;

        const WaveManager& getWaveManager() const;
        const std::vector<std::vector<Enemy*>>& getAliveEnemies() const;
	};
}

#endif