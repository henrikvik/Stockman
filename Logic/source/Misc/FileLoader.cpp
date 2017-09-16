#include <Misc/FileLoader.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <ctype.h>

#define LINE_END ';'
#define LINE_ASSIGN ':'
#define STRING '"'
#define FLOAT 'f'
#define START '{'
#define END '}'

#define FILE_PATH "Resources/Data/"
#define FILE_EXT ".lw"

using namespace Logic;

FileLoader::FileLoader()
{
}


FileLoader::~FileLoader()
{
}

int FileLoader::parseString(LoadedStruct &loaded, std::string const &str)
{
	std::istringstream stringStreamBlock(str.substr(0, str.find(END) - 1));
	std::string line, name, data;
	int index = 0; // test

	// optimize, but shouldnt be a problem with load on startup and small files
	while (getline(stringStreamBlock, line, LINE_END))
	{
		index = line.find(STRING) + 1;
		name = line.substr(index, line.find(STRING, index) - index);

		index = line.find(LINE_ASSIGN);
		index = line.find_first_not_of(" \t\r\n", index + 1);
		data = line.substr(index);

		switch (data[data.size() - 1])
		{
			case STRING:
				loaded.strings[name] = data.substr(1, data.size() - 1);
				break;
			case FLOAT:
				loaded.floats[name] = std::stof(data.substr(0, data.size() - 1));
				break;
			default:
				loaded.ints[name] = std::stoi(data);
				break;
		}
	}
	return 0;
}

int FileLoader::loadStructsFromFile(std::vector<LoadedStruct> &loadedStructs, std::string const &fileName, int offset, int fileOffset, int filePadding)
{
	std::ifstream inf(FILE_PATH + fileName + FILE_EXT);
	if (!inf.is_open())
		return -1; // see .h for error stuff

	std::string temp;
	while (getline(inf, temp, START))
	{
		loadedStructs.push_back(LoadedStruct());
		if (parseString(loadedStructs[loadedStructs.size() - 1], temp) == -1)
			return -2;
	}

	return 0;
}