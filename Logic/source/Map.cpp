#include "Map.h"
#include <Physics/Physics.h>        
#include <Keyboard.h>
#include <Graphics\include\Structs.h>
#include <Graphics\include\Utility\DebugDraw.h>
#include <Misc\RandomGenerator.h>

using namespace Logic;

Map::Map() { }

Map::~Map() 
{
	clear();
}

// Saves physics ptr & loads the map
void Map::init(Physics* physics, std::string path)
{
    // Saves physics pointer
    m_physicsPtr = physics;

    // Disables debug draw as default
    m_drawDebug = false;

    // Loads map from file (currently only hardcoded)
    loadMapFromFile(path);
}

// Deallocates all memory
void Map::clear()
{
	for (size_t i = 0; i < m_props.size(); i++)     delete m_props[i];
	for (size_t i = 0; i < m_hitboxes.size(); i++)  delete m_hitboxes[i];
    for (size_t i = 0; i < m_lights.size(); i++)    delete m_lights[i];

	m_props.clear();
	m_hitboxes.clear();
    m_lights.clear();
}

// If user holds tab, draw debug info
void Map::update(float deltaTime)
{
    m_drawDebug = DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift) ? true : false;
}

void Map::render() const
{
	for (Object* o : m_props)           o->render();
    for (LightObject* l : m_lights)     l->render();
    for (StaticObject* e : m_hitboxes)  e->render(); // Hitboxes should not be visiable at all at release

	// Drawing hitboxes debugged & lights
    if (m_drawDebug)
    {
        for (StaticObject* e : m_hitboxes)
            e->renderD();
     //   for (LightObject* l : m_lights)
     //       Graphics::Debug::PointLight(*l);
    }
}
	
std::vector<StaticObject*>*			Map::getProps()				{ return &m_props;				}
std::vector<StaticObject*>*			Map::getHitboxes()			{ return &m_hitboxes;			}
std::vector<LightObject*>*			Map::getLights()            { return &m_lights;             }

// Loads a map from a specific file
void Map::loadMapFromFile(std::string path)
{
    // Temp campfire map, remove this when an actual campfire is done
    if (path == "Campfire.txt")
    {
        std::vector<FrameHitbox> hitboxes; std::vector<FrameLight> lights; lights.push_back({ DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(1, 0.5, 0.3), 0.75f, 25.f });
        for (int i = 0; i < 50; i++) hitboxes.push_back({ { 0, (-5) +  i * 2.f, 3.f },{ (-5) + (i * 360.f / 100), (i * 360.f / 100), (i * 360.f / 100) },{ RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5) }, Resources::Models::UnitCube });
        for (int i = 0; i < 13; i++) hitboxes.push_back({ { 10, (-2) + i * 2.f, 8.f },{ (-10) + (i * 360.f / 100), (i * 360.f / 100), (i * 360.f / 100) },{ RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5) }, Resources::Models::UnitCube });
        for (int i = 0; i < 20; i++) hitboxes.push_back({ { 3, (-2) + i * 2.f, 4.f },{ (-6) + (i * 360.f / 100), (i * 360.f / 100), (i * 360.f / 100) },{ RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5) }, Resources::Models::UnitCube });
        for (int i = 0; i < 22; i++) hitboxes.push_back({ { -3, (-2) + i * 2.f, 4.f },{ (-6) +(i * 360.f / 100), (i * 360.f / 100), (i * 360.f / 100) },{ RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5) }, Resources::Models::UnitCube });
        for (int i = 0; i < 10; i++) hitboxes.push_back({ { -10, (-2) + i * 2.f, 8.f },{ (-10) + (i * 360.f / 100), (i * 360.f / 100), (i * 360.f / 100) },{ RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5), RandomGenerator::singleton().getRandomFloat(1, 1.5) }, Resources::Models::UnitCube });
        for (size_t i = hitboxes.size(); i--;) add(hitboxes[i]); for (size_t i = lights.size(); i--;) add(lights[i]);
        return;
    }

    // Loads hitboxes
    std::vector<FrameHitbox> hitboxes;
    hitboxes.push_back({ { 0, -10, 0 }, {0, 0, 0}, {500.f, 10, 500.f}, Resources::Models::UnitCube });
    hitboxes.push_back({ { 60, 0.75, 60 },{ 0, 0, 0 },{ 45, 0.75, 45 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 60, 2.00, 60 },{ 0, 0, 0 },{ 10, 2.00, 10 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 45, 1.5f, 45 },{ 0, 0, 0 },{ 10, 1.5f, 10 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 80, 3, 80 },{ 0, 0, 0 },{ 15, 3, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 50, 1, 80 },{ 0, 90, 90 },{ 15, 3, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 45, 1.5f, 45 },{ 0, 0, 0 },{ 10, 1.5f, 10 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 80, 1, 40 },{ 40, -90, -90 },{ 15, 3, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 120, 1, 180 },{ 40, 0, -90 },{ 60, 10, 45 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 125, 5, 100 },{ 0, 0, 0 },{ 15, 5, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 100, 4, 100 },{ 0, 0, 0 },{ 15, 4, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 120, 4, 60 },{ 0, 0, 0 },{ 15, 4, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 130, 4, 110 },{ 45, 0, 45 },{ 15, 4, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 150, 6, 150 },{ 0, 0, 0 },{ 40, 6, 40 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 60, 80, 60 },{ 0, 0, 0 },{ 45, 0.75, 45 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 45, 70, 45 },{ 0, 0, 0 },{ 10, 1.5f, 10 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 60, 50, 60 },{ 0, 0, 0 },{ 10, 2, 10 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 80, 42, 80 },{ 0, 0, 0 },{ 15, 3, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 50, 40, 80 },{ 0, 90, 90 },{ 15, 3, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 125, 35, 100 },{ 0, 0, 0 },{ 15, 5, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 100, 40, 100 },{ 0, 0, 0 },{ 15, 4, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 120, 50, 60 },{ 0, 0, 0 },{ 15, 4, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 130, 40, 110 },{ 45, 0, 45 },{ 15, 4, 15 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { 150, 60, 150 },{ 0, 0, 0 },{ 40, 6, 40 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { -60, 1, -60 },{ 0, 0.3f, 0 },{ 25, 3, 25 }, Resources::Models::UnitCube });

    // Loads lights
    std::vector<FrameLight> lights;
    lights.push_back({ DirectX::SimpleMath::Vector3(-35, 2, 35), DirectX::SimpleMath::Vector3(1, 1, 0), 0.75f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(-35, 2, -35), DirectX::SimpleMath::Vector3(1, 0, 1), 0.75f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(35, 2, -35), DirectX::SimpleMath::Vector3(0, 1, 1), 0.75f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(35, 2, 35), DirectX::SimpleMath::Vector3(1, 0.25f, 0.5f), 0.75f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(-23, 3, 74), DirectX::SimpleMath::Vector3(0.75, 0.25f, 0.5f), 0.85f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(-22, 3, 70), DirectX::SimpleMath::Vector3(1, 0.55f, 0.5f), 0.85f, 10.f });

    // Create everything and save
    for (size_t i = hitboxes.size(); i--;)  add(hitboxes[i]);
    for (size_t i = lights.size(); i--;)    add(lights[i]);
}

// Adds a pointlight to the map
void Map::add(FrameLight frameLight)
{
    m_lights.push_back(new LightObject(frameLight));
}

// Adds a visual prop to the map
void Map::add(FrameProp frameProp)
{
//    m_props.push_back(new StaticObject(frameProp.modelID));
}

// Adds a static hitbox to the map
void Map::add(FrameHitbox frameHitbox)
{
    m_hitboxes.push_back(new StaticObject(frameHitbox.modelID, m_physicsPtr->createBody(
        /* Shape */             Cube(frameHitbox.position, frameHitbox.rotation, frameHitbox.dimensions),
        /* Mass */              NULL,
        /* Sensor */            false,
        /* Collision Type */    Physics::COL_HITBOX,
        /* Collides With */     Physics::COL_EVERYTHING),
        /* Graphical Scaling */ frameHitbox.dimensions));
}