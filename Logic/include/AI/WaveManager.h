#ifndef WAVE_MANAGER_H
#define WAVE_MANAGER_H

#include <string>
#include <vector>
#include <sstream>
#include <Misc\FileLoader.h>

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
        public: // all structs
            struct WaveInformation
            {
                int nrOfWaves, startWave, bossTimeBonus;
                float timeMul, timeBonus;
                std::vector<int> bossWaves;
            };

            struct Entity {
                int id;
                float x, y, z;
                std::vector<int> effects;
            };

            struct EntitiesInWave
            {
                std::vector<int> enemies;
                std::vector<Entity> triggers, bosses;
            };
		private:
			std::string m_waveFileName;
			std::vector<FileLoader::LoadedStruct> m_waveFileLoaded;

            // index is the wave index, and offset is where it is after
            Entity loadEntity(int index, int offset) const;
            std::vector<int> splitInts(std::string const &item, const char delim) const;

            WaveInformation m_waveInformation;
		public:
			WaveManager(std::string waveFileName = "");
			~WaveManager();

			void loadFile();
			EntitiesInWave getEntities(int waveId) const;

			void setName(std::string name);
			std::string getWaveFileName() const;

            // info about all waves
            WaveInformation getWaveInformation() const;
            float getTimeForWave(int waveId) const;
	};
};

#endif