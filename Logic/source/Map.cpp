#include "Map.h"

using namespace Logic;

Map::Map() { }

Map::~Map() 
{
	clear();
}

void Map::init(Physics* physics, Player* player)
{
//	initProps();
	initHitboxes(physics);
//	initObjects(physics);			// Not used as intented as for rn, should only create non-moving objects, not entities
//	initGrapplingPoints(physics, player);

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

	//Entity* headboxTest = new TestHeadShot(physics->createBody(Cube({ 30, 3, 5 }, { 0, 0, 0 }, { 1, 1, 1}), 0.f, false), { 1, 1, 1 });
	//m_hitboxes.push_back(headboxTest);

	Entity* house;

	house = new Entity(physics->createBody(Cube({ 60, 0.75, 60 }, { 0, 0, 0 }, { 45, 0.75, 45 }), 0.f, false), { 45, 1.5f, 45 });
	m_hitboxes.push_back(house);
	
	house = new Entity(physics->createBody(Cube({ 45, 1.5f, 45 }, { 0, 0, 0 }, { 10, 1.5f, 10 }), 0.f, false), { 10, 1.5f, 10 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 60, 2, 60 }, { 0, 0, 0 }, { 10, 2, 10 }), 0.f, false), { 10, 2, 10 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 80, 3, 80 }, { 0, 0, 0 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 50, 1, 80 }, { 0, 90, 90 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 80, 1, 40 }, { 40, -90, -90 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 120, 1, 180 }, { 40, 0, -90 }, { 60, 10, 45 }), 0.f, false), { 60, 10, 45 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 125, 5, 100 }, { 0, 0, 0 }, { 15, 5, 15 }), 0.f, false), { 15, 5, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 100, 4, 100 }, { 0, 0, 0 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 120, 4, 60 }, { 0, 0, 0 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 130, 4, 110 }, { 45, 0, 45 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 150, 6, 150 }, { 0, 0, 0 }, { 40, 6, 40 }), 0.f, false), { 40, 6, 40 });
	m_hitboxes.push_back(house); 

	house = new Entity(physics->createBody(Cube({ 60, 80, 60 }, { 0, 0, 0 }, { 45, 0.75, 45 }), 0.f, false), { 45, 1.5f, 45 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 45, 70, 45 }, { 0, 0, 0 }, { 10, 1.5f, 10 }), 0.f, false), { 10, 1.5f, 10 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 60, 50, 60 }, { 0, 0, 0 }, { 10, 2, 10 }), 0.f, false), { 10, 2, 10 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 80, 42, 80 }, { 0, 0, 0 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 50, 40, 80 }, { 0, 90, 90 }, { 15, 3, 15 }), 0.f, false), { 15, 3, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 125, 35, 100 }, { 0, 0, 0 }, { 15, 5, 15 }), 0.f, false), { 15, 5, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 100, 40, 100 }, { 0, 0, 0 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 120, 50, 60 }, { 0, 0, 0 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 130, 40, 110 }, { 45, 0, 45 }, { 15, 4, 15 }), 0.f, false), { 15, 4, 15 });
	m_hitboxes.push_back(house);

	house = new Entity(physics->createBody(Cube({ 150, 60, 150 }, { 0, 0, 0 }, { 40, 6, 40 }), 0.f, false), { 40, 6, 40 });
	m_hitboxes.push_back(house);

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

// Create the grappling points (Need to be spheres for optimization)
void Map::initGrapplingPoints(Physics * physics, Player* player)
{
	btVector3 halfextent(2.f, 2.f, 2.f);
	GrapplingPoint* grappling = new GrapplingPoint(physics->createBody(Sphere({ -20, 20, 5 }, { 0, 0, 0 }, 4.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -15, 40, -5 }, { 0, 0, 0 }, 4.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -20, 60, 0 }, { 0, 0, 0 }, 4.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -30, 73, -30 }, { 0, 0, 0 }, 4.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -80, 73, -30 }, { 0, 0, 0 }, 4.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -30, 73, -80 }, { 0, 0, 0 }, 4.f), 0.f, true), halfextent, Graphics::SPHERE);
	grappling->init(physics, player);
	m_grapplingPoints.push_back(grappling);
	grappling = new GrapplingPoint(physics->createBody(Sphere({ -80, 73, -80 }, { 0, 0, 0 }, 4.f), 0.f, true), halfextent, Graphics::SPHERE);
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

	// Updating grappling hooks
	for (size_t i = 0; i < m_grapplingPoints.size(); i++)
		m_grapplingPoints[i]->update(deltaTime);
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
