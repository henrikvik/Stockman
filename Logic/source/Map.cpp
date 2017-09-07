#include "Map.h"

using namespace Logic;

Map::Map()
{

}

Map::~Map() { }

void Map::clear()
{
	props.clear();
//	hitboxes.clear();
}

void Map::addProp()
{
	
}

std::vector<Object*>* Map::getProps()
{
	return &props;
}
