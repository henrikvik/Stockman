#include "Map.h"

using namespace Logic;

Map::Map() { }

Map::~Map() 
{
	clear();
}

void Map::init(Physics* physics)
{
	initProps();
	initHitboxes(physics);

	m_drawHitboxes = true;
}

void Map::initProps()
{

}

void Map::initHitboxes(Physics* physics)
{
//	Hitbox* h = new Hitbox();
//	h->init(physics, BodyDesc(0, { 0, 0, 0 }, { 0, 1, 0 }));
//	m_hitboxes.push_back(h);
}

void Map::clear()
{
	for (size_t i = 0; i < m_props.size(); i++)
		delete m_props[i];

	for (size_t i = 0; i < m_props.size(); i++)
		delete m_hitboxes[i];

	m_props.clear();
	m_hitboxes.clear();
}

void Map::render(RenderRegister & renderRegister)
{
	// Drawing props
	for (size_t i = 0; i < m_props.size(); i++)
		m_props[i]->render(renderRegister);

	// Drawing hitboxes
	if (m_drawHitboxes)
		for (size_t i = 0; i < m_hitboxes.size(); i++)
			m_hitboxes[i]->render(renderRegister);
}

std::vector<Object*>* Map::getProps()			{	return &m_props;		}
std::vector<Hitbox*>* Logic::Map::getHitboxes() {	return &m_hitboxes;	}