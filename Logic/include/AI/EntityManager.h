#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>

#include <AI/Enemy.h>
#include <AI/WaveManager.h>
#include <AI/TriggerManager.h>

#include <Player\Player.h>

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
		std::vector<Enemy*> m_enemies, m_bossEnemies, m_deadEnemies;
		std::vector<double> time;

		TriggerManager m_triggerManager;
		WaveManager m_waveManager;
		int m_currentWave, m_frame;

		void reserveData(); // reserve space in vectors
	public:
		EntityManager();
		EntityManager(EntityManager const &entityManager) = delete;
		~EntityManager();

		void update(Player const &player, float deltaTime);
		void clear();

		void spawnWave(Physics &physics);

		void setCurrentWave(int currentWave);
		void render(Graphics::Renderer &renderer);

		int getEnemiesAlive() const;
		int getCurrentWave() const;

		EntityManager* operator=(EntityManager const &entityManager) = delete;
	};
}

#endif