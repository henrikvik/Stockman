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
	public:
		static FileLoader& singleton() {
			static FileLoader ins;
			return ins;
		} // might be better to not be singleton but felt simpler at the time
	private:
		static FileLoader* instance;
	public:
		FileLoader();
		virtual ~FileLoader();

		/* @returns: 0 on success, -1 on failure to find file, -2 on failure to read file
		not exceptions due to various reasons. */
		int loadStructFromFile(void *structs, int size, std::string const &fileName, int offset = 0, int fileOffset = 0);
	};
}

#endif