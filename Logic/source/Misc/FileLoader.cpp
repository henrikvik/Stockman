#include <Misc/FileLoader.h>

#include <fstream>
#include <iostream>

#define SEPARATOR ','
#define START '{'
#define END '}'

#define FILE_PATH "Resources/Data/"

using namespace Logic;

FileLoader::FileLoader()
{
}


FileLoader::~FileLoader()
{
}

int FileLoader::loadStructFromFile(void* structs, int size, std::string const &fileName, int offset, int fileOffset)
{
	std::ifstream inf(FILE_PATH + fileName);
	if (!inf.is_open())
		return -1; // see .h for error stuff

	std::string temp;
	while (getline(inf, temp, START)) 
	{
		if (fileOffset > 0) fileOffset--;
		else 
		{
			printf("str: %s", temp.c_str());
		}
	}

	return 0;
}