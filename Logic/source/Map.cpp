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
	Entity* infinite = new Entity(physics->createBody(Plane({ 0, 1, 0 }), 0, false));
    infinite->setWorldTranslation(DirectX::SimpleMath::Matrix::CreateScale(1000, 0.0001, 1000));

    m_hitboxes.push_back(infinite);
}

void Map::initObjects(Physics * physics)
{
	for (int i = 0; i < 5; ++i)
	{
		Entity* box = new Entity(physics->createBody(Cube({ 0, 10 , 0 }, { 0, 0 , 0 }, { 1, 1 , 1 }), 0.5f, false));
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

void Map::render(Graphics::Renderer& renderer)
{
	// Drawing props
	for (Object* o : m_props)
		o->render(renderer);

	// Drawing hitboxes
	for (Entity* e : m_objects)
		e->render(renderer);

	// Drawing hitboxes
	if (m_drawHitboxes)
		for (Entity* e : m_hitboxes)
			e->render(renderer);
}

std::vector<Object*>* Map::getProps()		{	return &m_props;		}
std::vector<Entity*>* Map::getHitboxes()	{	return &m_hitboxes;		}
std::vector<Entity*>* Map::getObjects()		{	return &m_objects;		}