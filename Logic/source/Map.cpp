#include "Map.h"

using namespace Logic;

Map::Map() { }

Map::~Map() 
{
	clear();
}

void Map::init(Physics* physics, Player* player)
{
	initProps();
	initHitboxes(physics);
	initObjects(physics);			// Not used as intented as for rn, should only create non-moving objects, not entities
	initGrapplingPoints(physics, player);

	m_drawHitboxes = true;
}

void Map::initProps()
{

}

void Map::initHitboxes(Physics* physics)
{
	Entity* infinite = new Entity(physics->createBody(Plane({ 0, 1, 0 }), 0, false), btVector3(1000, 0.0001, 1000), Graphics::CUBE); 
    m_hitboxes.push_back(infinite);
	//Entity* secondinfinite = new Entity(physics->createBody(Plane({ 0, 0, 1 }), 0, false), btVector3(1000, 0.0001, 1000));
	//m_hitboxes.push_back(secondinfinite);
}

void Map::initObjects(Physics * physics)
{
	for (int i = 0; i < 5; ++i)
	{
		btVector3 halfextent(1.0 * (i + 1), 1.0, 1.0);
		Entity* box = new Entity(physics->createBody(Cube({ 1.f * (i * 0.25f + 1), 5, 0 }, { 0, 0, 0 }, halfextent), 1.f, false), halfextent);
		m_objects.push_back(box);
	}
}

void Map::initGrapplingPoints(Physics * physics, Player* player)
{
	btVector3 halfextent(1.f, 1.f, 1.f);
	GrapplingPoint* grappling = new GrapplingPoint(physics->createBody(Sphere({ -20, 20, 5 }, { 0, 0, 0 }, 2.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -15, 40, -5 }, { 0, 0, 0 }, 2.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -20, 60, 0 }, { 0, 0, 0 }, 2.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -30, 73, -30 }, { 0, 0, 0 }, 2.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -80, 73, -30 }, { 0, 0, 0 }, 2.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -30, 73, -80 }, { 0, 0, 0 }, 2.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -80, 73, -80 }, { 0, 0, 0 }, 2.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
}

void Map::clear()
{
	for (size_t i = 0; i < m_props.size(); i++)
		delete m_props[i];

	for (size_t i = 0; i < m_hitboxes.size(); i++)
		delete m_hitboxes[i];

	for (size_t i = 0; i < m_objects.size(); i++)
		delete m_objects[i];

	for (size_t i = 0; i < m_grapplingPoints.size(); i++)
		delete m_grapplingPoints[i];

	m_props.clear();
	m_hitboxes.clear();
	m_objects.clear();
	m_grapplingPoints.clear();
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
	for (Entity* e : m_objects)
		e->render(renderer);

	// Drawing grappling points
	for (GrapplingPoint* g : m_grapplingPoints)
		g->render(renderer);

	// Drawing hitboxes
	if (m_drawHitboxes)
		for (Entity* e : m_hitboxes)
			e->render(renderer);
}

std::vector<Object*>*			Map::getProps()				{ return &m_props;				}
std::vector<Entity*>*			Map::getHitboxes()			{ return &m_hitboxes;			}
std::vector<Entity*>*			Map::getObjects()			{ return &m_objects;			}
std::vector<GrapplingPoint*>*	Map::getGrapplingPoints()	{ return &m_grapplingPoints;	}
