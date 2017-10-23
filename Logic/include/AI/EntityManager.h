#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <thread>

#include <AI/Enemy.h>
#include <AI/WaveManager.h>
#include <AI/TriggerManager.h>
#include <Misc/ComboMachine.h>

#include <Player\Player.h>
#include <Projectile\ProjectileManager.h>

#include <Graphics\include\Renderer.h>
#include <Physics\Physics.h>

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
	class EntityManager
	{
	private:
		static const int NR_OF_THREADS = 8;

		std::vector<std::vector<Enemy*>> m_enemies;
		std::vector<Enemy*> m_deadEnemies;
		std::vector<double> time;

		std::thread *threads[NR_OF_THREADS];
		int m_indexRunning[NR_OF_THREADS];
		bool m_threadRunning[NR_OF_THREADS];

		TriggerManager m_triggerManager;
		WaveManager m_waveManager;
		int m_currentWave, m_frame;
		float m_deltaTime;

		void deleteData(); // delete data in vectors
		void allocateData(); // resize enemy vector 

		void resetThreads();
		void deleteThreads();
		void joinAllThreads();
		void deleteThread(std::thread *t);
	public:

		EntityManager();
		EntityManager(EntityManager const &entityManager) = delete;
		~EntityManager();

		void update(Player const &player, float deltaTime);
		void updateEnemies(int index, Player const &player, float deltaTime);
		// statis because threads will use this
		static void updateEnemiesAndPath(EntityManager *manager, int index, Player const &player, float deltaTime);
		static void onPathThreadCreation(EntityManager *manager, int index, Player const &player, float deltaTime);
		void updateEnemy(Enemy *enemy, int index, Player const &player, float deltaTime);
		void clear();

		int giveEffectToAllEnemies(StatusManager::EFFECT_ID id);

		void spawnWave(Physics &physics, ProjectileManager *projectiles);
		void spawnEnemy(Enemy::ENEMY_TYPE id, btVector3 const &pos, std::vector<int> const &effects,
			Physics &physics, ProjectileManager *projectiles);
		void spawnTrigger(int id, btVector3 const &pos, std::vector<int> &effects,
			Physics &physics, ProjectileManager *projectiles);

		void setCurrentWave(int currentWave);
		void render(Graphics::Renderer &renderer);

		size_t getEnemiesAlive() const;
		int getCurrentWave() const;

		EntityManager* operator=(EntityManager const &entityManager) = delete;
	};
}

#endif