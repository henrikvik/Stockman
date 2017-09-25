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
#define SPACE " \t\r\n"

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
		index = line.find(STRING) + 1; // find beginning of name
		name = line.substr(index, line.find(STRING, index) - index); // extract name

		index = line.find(LINE_ASSIGN); // find line assignment ':'
		index = line.find_first_not_of(SPACE, index + 1); // find the data
		data = line.substr(index); // substract data

		switch (data[data.size() - 1]) // save data
		{
			case STRING:
				loaded.strings[name] = data.substr(1, data.size() - 2);
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
	inf.seekg(1); // first symbol should always be { so that is why. That can be skipped. Better solution would be to find it and use the index, but unnecessary for the moment
	for (int i = 0; i < fileOffset; i++) getline(inf, temp, START); // go through offset

	while (getline(inf, temp, START))
	{
		loadedStructs.push_back(LoadedStruct());
		if (parseString(loadedStructs[loadedStructs.size() - 1], temp) == -1)
			return -2;
		for (int i = 0; i < filePadding; ++i) getline(inf, temp, START); // go through padding
	}

	inf.close();

	return 0;
}

int FileLoader::saveStructsToFile(std::vector<LoadedStruct> &loadedStructs, std::string const &fileName) {
	std::ofstream outFile(FILE_PATH + fileName + FILE_EXT);
	outFile.clear();

	if (!outFile.is_open())
		return -1;

	for (auto const &aStruct : loadedStructs) { // simple loop through everything and print it to file
		outFile << START << "\n";

		for (auto const &iterating : aStruct.strings) {
			outFile << "\t" << STRING << iterating.first << STRING << LINE_ASSIGN
				<< " " << STRING << iterating.second << STRING << LINE_END << "\n";
		}

		for (auto const &iterating : aStruct.ints) {
			outFile << "\t" << STRING << iterating.first << STRING << LINE_ASSIGN
				<< " " << iterating.second << LINE_END << "\n";
		}

		for (auto const &iterating : aStruct.floats) {
			outFile << "\t" << STRING << iterating.first << STRING << LINE_ASSIGN
				<< " " << iterating.second << FLOAT << LINE_END << "\n";
		}

		outFile << END << "\n";
	}

	outFile.close();

	return 0;
}