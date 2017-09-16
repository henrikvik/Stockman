#ifndef FILELOADER_H
#define FILELOADER_H

#include <string>
#include <vector>
#include <unordered_map>

#pragma region ClassDesc
	/*

		CLASS: FileLoader
		AUTHOR: Lukas Westling
		Desc: Singleton to load files with some helper methods
			
	*/
#pragma endregion

namespace Logic {
	class FileLoader
	{
	public:
		struct LoadedStruct
		{
			std::unordered_map<std::string, std::string> strings;
			std::unordered_map<std::string, int> ints;
			std::unordered_map<std::string, float> floats;
		};

		static FileLoader& singleton() {
			static FileLoader ins;
			return ins;
		} // might be better to not be singleton but felt simpler at the time
	private:
		static FileLoader* instance;
		// parse string to structs at index
		int parseString(LoadedStruct &loaded, std::string const &str);
	public:
		FileLoader();
		virtual ~FileLoader();

		/* @returns: 0 on success, -1 on failure to find file, -2 on failure to read file
		not exceptions due to various reasons. */
		int loadStructsFromFile(std::vector<LoadedStruct> &loadedStructs, std::string const &fileName, int offset = 0, int fileOffset = 0, int filePadding = 0);
	};
}

#endif