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
		private:
			std::string m_waveFileName;
			std::vector<FileLoader::LoadedStruct> m_waveFileLoaded;
			std::istringstream strstream;
		public:
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

			WaveManager(std::string waveFileName = "");
			~WaveManager();

			void loadFile();
			EntitiesInWave getEntities(int waveId);

			void setName(std::string name);
			std::string getWaveFileName() const;
		private:
			// index is the wave index, and offset is where it is after
			// look at wave.lw it makes sense
			Entity loadEntity(int index, int offset);
			std::vector<int> splitInts(std::string const &item, const char delim);
	};
};

#endif