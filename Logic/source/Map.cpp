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
	initObjects(physics);

	m_drawHitboxes = true;
}

void Map::initProps()
{

}

void Map::initHitboxes(Physics* physics)
{
	Entity* infinite = new Entity(physics->addBody(Plane({ 0, 1, 0 }), 0, false));
    infinite->setWorldMatrix(DirectX::SimpleMath::Matrix::CreateScale(1000, 0.01, 1000));

    m_hitboxes.push_back(infinite);
}

void Map::initObjects(Physics * physics)
{
	for (int i = 0; i < 5; ++i)
	{
		Entity* box = new Entity(physics->addBody(Cube({ 0, 10 , 0 }, { 0, 0 , 0 }, { 0.5, 0.5 , 0.5 }), 25.f, false));
		m_objects.push_back(box);
	}
}

void Map::clear()
{
	for (size_t i = 0; i < m_props.size(); i++)
		delete m_props[i];

	for (size_t i = 0; i < m_hitboxes.size(); i++)
		delete m_hitboxes[i];

	for (size_t i = 0; i < m_objects.size(); i++)
		delete m_objects[i];

	m_props.clear();
	m_hitboxes.clear();
	m_objects.clear();
}

void Map::update(float deltaTime)
{
	// Updating interactable objects
	for (size_t i = 0; i < m_objects.size(); i++)
		m_objects[i]->update(deltaTime);
}

void Map::render(RenderRegister & renderRegister)
{
	// Drawing props
	for (size_t i = 0; i < m_props.size(); i++)
		m_props[i]->render(renderRegister);

	// Drawing hitboxes
	for (size_t i = 0; i < m_objects.size(); i++)
		m_objects[i]->render(renderRegister);

	// Drawing hitboxes
	if (m_drawHitboxes)
		for (size_t i = 0; i < m_hitboxes.size(); i++)
			m_hitboxes[i]->render(renderRegister);
}

std::vector<Object*>* Map::getProps()		{	return &m_props;		}
std::vector<Entity*>* Map::getHitboxes()	{	return &m_hitboxes;		}
std::vector<Entity*>* Map::getObjects()		{	return &m_objects;		}