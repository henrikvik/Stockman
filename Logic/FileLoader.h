#ifndef FILELOADER_H
#define FILELOADER_H

#include <string>

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
	private:
		static FileLoader* instance;
	public:
		FileLoader();
		virtual ~FileLoader();

		/* @returns: 0 on success, -1 on failure to find file, -2 on failure to read file
		not exceptions due to various reasons. */
		int loadStructFromFile(struct structs[], int size, std::string const &fileName, int offset = 0, int fileOffset = 0);

		static FileLoader& singleton(); // might be better to not be singleton but felt simpler at the time
	};
}

#endif