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
	StaticObject* infinite = new StaticObject(Graphics::CUBE, physics->createBody(Plane({ 0, 1, 0 }), 0, false), btVector3(1000, 0.0001, 1000));
	m_hitboxes.push_back(infinite);

	StaticObject* house;

	house = new StaticObject(Graphics::CUBE, physics->createBody(Sphere({ -20, 3, 10 }, { 0, 0, 0 }, 3.f), 0.f, false), { 1.5f, 1.5f, 1.5f });
	house->setModelID(Graphics::SPHERE);
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Sphere({ -30, 5, 10 }, { 0, 0, 0 }, 1.f), 0.f, false), { 0.5f, 0.5f, 0.5f });
	house->setModelID(Graphics::SPHERE);
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 60, 0.75, 60 }, { 0, 0, 0 }, { 45, 0.75, 45 }), 0.f, false), { 45, 1.5f, 45 });
	m_hitboxes.push_back(house);
	
	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 45, 1.5f, 45 }, { 0, 0, 0 }, { 10, 1.5f, 10 }), 0.f, false), { 10, 1.5f, 10 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 60, 2, 60 }, { 0, 0, 0 }, { 10, 2, 10 }), 0.f, false), { 10, 2, 10 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 80, 3, 80 }, { 0, 0, 0 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 50, 1, 80 }, { 0, 90, 90 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 80, 1, 40 }, { 40, -90, -90 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 120, 1, 180 }, { 40, 0, -90 }, { 60, 10, 45 }), 0.f, false), { 60, 10, 45 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 125, 5, 100 }, { 0, 0, 0 }, { 15, 5, 15 }), 0.f, false), { 15, 5, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 100, 4, 100 }, { 0, 0, 0 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 120, 4, 60 }, { 0, 0, 0 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 130, 4, 110 }, { 45, 0, 45 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 150, 6, 150 }, { 0, 0, 0 }, { 40, 6, 40 }), 0.f, false), { 40, 6, 40 });
	m_hitboxes.push_back(house); 

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 60, 80, 60 }, { 0, 0, 0 }, { 45, 0.75, 45 }), 0.f, false), { 45, 1.5f, 45 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 45, 70, 45 }, { 0, 0, 0 }, { 10, 1.5f, 10 }), 0.f, false), { 10, 1.5f, 10 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 60, 50, 60 }, { 0, 0, 0 }, { 10, 2, 10 }), 0.f, false), { 10, 2, 10 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 80, 42, 80 }, { 0, 0, 0 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 50, 40, 80 }, { 0, 90, 90 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 125, 35, 100 }, { 0, 0, 0 }, { 15, 5, 15 }), 0.f, false), { 15, 5, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 100, 40, 100 }, { 0, 0, 0 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 120, 50, 60 }, { 0, 0, 0 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 130, 40, 110 }, { 45, 0, 45 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 150, 60, 150 }, { 0, 0, 0 }, { 40, 6, 40 }), 0.f, false), { 40, 6, 40 });
	m_hitboxes.push_back(house);

	house = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ -60, 6, -60 }, { 0.3f, 0, 0 }, { 25, 3, 25 }), 0.f, false), { 25, 3, 25 });
	m_hitboxes.push_back(house);

}

void Map::initObjects(Physics * physics)
{
	btVector3 halfextent(1.0, 1.0, 1.0);
	StaticObject* box = new StaticObject(Graphics::CUBE, physics->createBody(Cube({ 5, 3, -5 }, { 0, 0, 0 }, halfextent), 1.f, false), halfextent);
	box->addExtraBody(physics->createBody(Sphere({ 0, 0, 0 }, { 0, 0, 0 }, 1.f), 0.f, true), 2.f, { 0, 3.f, 0 });
	m_objects.push_back(box);
	
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

	// Drawing objects
	for (StaticObject* e : m_objects)
		e->render(renderer);

	// Drawing hitboxes
	if (m_drawHitboxes)
		for (StaticObject* e : m_hitboxes)
			e->render(renderer);
}
	
std::vector<Object*>*				Map::getProps()				{ return &m_props;				}
std::vector<StaticObject*>*			Map::getHitboxes()			{ return &m_hitboxes;			}
std::vector<StaticObject*>*			Map::getObjects()			{ return &m_objects;			}
