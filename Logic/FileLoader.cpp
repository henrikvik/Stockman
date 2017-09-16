#include "FileLoader.h"
#include <fstream>
#include <iostream>

#define SEPARATOR ','
#define START '{'
#define END '}'

using namespace Logic;

FileLoader::FileLoader()
{
}


FileLoader::~FileLoader()
{
}

int loadStructFromFile(struct structs[], int size, std::string const &fileName, int offset, int fileOffset) 
{
	std::ifstream inf(fileName);
	if (!inf.is_open())
		return -1; // see .h for error stuff

	std::string temp;
	while (getline(inf, temp, START)) 
	{
		if (fileOffset > 0) fileOffset--;
		else 
		{
			printf("str: %s", temp);
		}
	}

	return 0;
}


FileLoader& FileLoader::singleton() 
{
	static FileLoader stLoader;
	return stLoader;
}