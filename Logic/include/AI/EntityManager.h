#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <AI/Enemy.h>
#include <AI/WaveManager.h>

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
		std::vector<bool> m_enemiesDeadBool; // For performence
		std::vector<Enemy*> m_enemies;

		WaveManager m_waveManager;
		int m_currentWave;

	public:
		EntityManager();
		EntityManager(EntityManager const &entityManager) = delete;
		~EntityManager();

		void update(float deltaTime);
		void clear();

		void spawnWave();

		void setCurrentWave(int currentWave);

		int getEnemiesAlive() const;
		int getCurrentWave() const;

		EntityManager* operator=(EntityManager const &entityManager) = delete;
	};
}

#endif