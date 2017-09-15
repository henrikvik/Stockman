#ifndef WAVE_MANAGER_H
#define WAVE_MANAGER_H

#include <string>
#include <vector>

#pragma region ClassDesc
	/*
		CLASS: WaveManager
		AUTHOR: Lukas Westling

		This class loads the wave from a file,
		and is used by EntityManager to spawn the
		wave.
	*/
#pragma endregion

namespace Logic
{
	class WaveManager
	{
		private:
			std::string m_waveFileName;
		public:
			WaveManager(std::string waveFileName = "");
			~WaveManager();

			std::vector<int> getEnemies(int waveId);

			void setName(std::string name);
			std::string getWaveFileName() const;
	};
}

#endif