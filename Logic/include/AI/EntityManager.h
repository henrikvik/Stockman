#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#pragma region Comment
/*
	CLASS: EntityManager (Bad name TODO: change)
	AUTHOR: Lukas Westling

	This class handles Enemies and how it spawns
	with the help of WaveManager.

	It also handles triggers and how it spawns. (WaveManager?)
*/
#pragma endregion ClassDesc

namespace Logic 
{
	class EntityManager
	{
	private:
		int m_currentWave;
	public:
		EntityManager();
		~EntityManager();

		void update(float deltaTime);
		void clear();

		void spawnWave();

		void setCurrentWave(int currentWave);

		int getEnemiesAlive() const;
		int getCurrentWave() const;
	};
}

#endif