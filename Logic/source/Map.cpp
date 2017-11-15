#include "Map.h"
#include <Physics/Physics.h>        
#include <Keyboard.h>
#include <Graphics\include\Structs.h>
#include <Graphics\include\Utility\DebugDraw.h>
#include <Misc\RandomGenerator.h>
#include <toml\toml.h>
#include <fstream>

using namespace Logic;

Map::Map() { }

Map::~Map() 
{
	clear();
}

void Map::add(FrameProp frameProp)
{
  //  m_objects.push_back(new Object(frameProp.modelID, frameProp.position, frameProp.rotation));
}

void Map::add(FrameHitbox frameHitbox)
{
    //if (frameHitbox.modelID == Resources::Models::UnitCube)
    //    m_hitboxes.push_back(new StaticObject(frameHitbox.modelID, m_physicsPtr->createBody(
    //        Cube(frameHitbox.position, frameHitbox.rotation, frameHitbox.dimensions), NULL, false,
    //        Physics::COL_HITBOX,
    //        Physics::COL_EVERYTHING),
    //        {1, 1.f, 1},
    //        StaticObject::NavigationMeshFlags::CULL
    //    ));
    //else
        m_hitboxes.push_back(new StaticObject(frameHitbox.modelID, m_physicsPtr->createBody(
            Cube(frameHitbox.position, frameHitbox.rotation, frameHitbox.dimensions), NULL, false,
            Physics::COL_HITBOX,
            Physics::COL_EVERYTHING),
            frameHitbox.dimensions,
            StaticObject::NavigationMeshFlags::NO_CULL
        ));
}

void Map::init(Physics* physics, std::string path)
{
    m_physicsPtr = physics;

    // Disables debug draw as default
    m_drawDebug = false;

    // Loads map from file (currently only hardcoded)
    loadMapFromFile(path);
    loadMap(Resources::Maps::Scene);
}

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
        std::vector<FrameHitbox> hitboxes; 
        std::vector<FrameLight> lights;

        hitboxes.push_back({ { 0, -10, 0 },{ 0, 0, 0 },{ 500.f, 10, 500.f },    Resources::Models::UnitCube });

        hitboxes.push_back({ { 0, 0.0f, 0 },{ 0, 0, 0 },{ 0.55f, 0.55f, 0.55f },    Resources::Models::UnitCube });
        hitboxes.push_back({ { 3, 0.0f, 0.5 },{ 0, 0, 0 },{ 1, 0.75, 1 },    Resources::Models::UnitCube });
        hitboxes.push_back({ { -3, 0.0f, -1 },{ 0, 35, 0 },{ 1, 0.75, 1 },    Resources::Models::UnitCube });
        hitboxes.push_back({ { 3, 1.0f, 0.5 },{ 0, 35, 0 },{ 1, 1, 1 },    Resources::Models::StaticSummon });
        hitboxes.push_back({ { -3, 1.0f, -1 },{ 0, 45, 0 },{ 1, 1, 1 },    Resources::Models::StaticSummon });

        for (size_t i = hitboxes.size(); i--;) add(hitboxes[i]); for (size_t i = lights.size(); i--;) add(lights[i]);
        return;
    }

    // Loads hitboxes
    std::vector<FrameHitbox> hitboxes;
    hitboxes.push_back({ { 0, -10, 0 }, {0, 0, 0}, {500.f, 10, 500.f},    Resources::Models::UnitCube });
    hitboxes.push_back({ { 60, 0.75, 60 },{ 0, 0, 0 },{ 45, 0.75, 45 },   Resources::Models::UnitCube });
    hitboxes.push_back({ { 60, 0.75, -160 },{ 0, 0, 0 },{ 45, 0.75, 45 }, Resources::Models::UnitCube });
    hitboxes.push_back({ { -160, 0.75, 60 },{ 0, 0, 0 },{ 45, 0.75, 45 }, Resources::Models::UnitCube });

    RandomGenerator &gen = RandomGenerator::singleton();
    // Insane random map generator, will make unbelievable maps, they look like they are placed by humans but it ACTUALLY CAN YOU BELIEVE IT: RANDOM!?!?!?!?!?!?!?!?!?. This will make Diablo 3 look like a little noob
    for (int i = 0; i < gen.getRandomInt(4, 13); i++)
    {
        hitboxes.push_back({ { gen.getRandomFloat(-150, 150), 0.75, gen.getRandomFloat(-150, 150) },{ 0, 0, 0 },{ gen.getRandomFloat(5, 35), 0.75, gen.getRandomFloat(5, 35) }, Resources::Models::UnitCube });
    }
    /*
    hitboxes.push_back({ { 60, 2.00, 60 },{ 0, 0, 0 },{ 10, 2.00, 10 }, Graphics::CUBE });
    hitboxes.push_back({ { 45, 1.5f, 45 },{ 0, 0, 0 },{ 10, 1.5f, 10 }, Graphics::CUBE });
    hitboxes.push_back({ { 80, 3, 80 },{ 0, 0, 0 },{ 15, 3, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 50, 1, 80 },{ 0, 90, 90 },{ 15, 3, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 45, 1.5f, 45 },{ 0, 0, 0 },{ 10, 1.5f, 10 }, Graphics::CUBE });
    hitboxes.push_back({ { 80, 1, 40 },{ 40, -90, -90 },{ 15, 3, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 120, 1, 180 },{ 40, 0, -90 },{ 60, 10, 45 }, Graphics::CUBE });
    hitboxes.push_back({ { 125, 5, 100 },{ 0, 0, 0 },{ 15, 5, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 100, 4, 100 },{ 0, 0, 0 },{ 15, 4, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 120, 4, 60 },{ 0, 0, 0 },{ 15, 4, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 130, 4, 110 },{ 45, 0, 45 },{ 15, 4, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 150, 6, 150 },{ 0, 0, 0 },{ 40, 6, 40 }, Graphics::CUBE });
    hitboxes.push_back({ { 60, 80, 60 },{ 0, 0, 0 },{ 45, 0.75, 45 }, Graphics::CUBE });
    hitboxes.push_back({ { 45, 70, 45 },{ 0, 0, 0 },{ 10, 1.5f, 10 }, Graphics::CUBE });
    hitboxes.push_back({ { 60, 50, 60 },{ 0, 0, 0 },{ 10, 2, 10 }, Graphics::CUBE });
    hitboxes.push_back({ { 80, 42, 80 },{ 0, 0, 0 },{ 15, 3, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 50, 40, 80 },{ 0, 90, 90 },{ 15, 3, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 125, 35, 100 },{ 0, 0, 0 },{ 15, 5, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 100, 40, 100 },{ 0, 0, 0 },{ 15, 4, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 120, 50, 60 },{ 0, 0, 0 },{ 15, 4, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 130, 40, 110 },{ 45, 0, 45 },{ 15, 4, 15 }, Graphics::CUBE });
    hitboxes.push_back({ { 150, 60, 150 },{ 0, 0, 0 },{ 40, 6, 40 }, Graphics::CUBE });
    hitboxes.push_back({ { -60, 1, -60 },{ 0, 0.3f, 0 },{ 25, 3, 25 }, Graphics::CUBE }); */

    // Loads lights
    std::vector<FrameLight> lights;
    lights.push_back({ DirectX::SimpleMath::Vector3(-35, 2, 35), DirectX::SimpleMath::Vector3(1, 1, 0), 0.75f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(-35, 2, -35), DirectX::SimpleMath::Vector3(1, 0, 1), 0.75f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(35, 2, -35), DirectX::SimpleMath::Vector3(0, 1, 1), 0.75f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(35, 2, 35), DirectX::SimpleMath::Vector3(1, 0.25f, 0.5f), 0.75f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(-23, 3, 74), DirectX::SimpleMath::Vector3(0.75, 0.25f, 0.5f), 0.85f, 10.f });
    lights.push_back({ DirectX::SimpleMath::Vector3(-22, 3, 70), DirectX::SimpleMath::Vector3(1, 0.55f, 0.5f), 0.85f, 10.f });

    // Create everything and save
    for (auto & hitbox : hitboxes) add(hitbox);
    for (auto & light : lights) add(light);
}

void Logic::Map::loadMap(Resources::Maps::Files map)
{
    toml::ParseResult mapFile = toml::parseFile(Resources::Maps::Paths.at(map));
    if (!mapFile.valid()) throw std::runtime_error(mapFile.errorReason);

    toml::Value mapValue = mapFile.value;

    struct Instance
    {
        std::string name;
        std::string model;
        btVector3 translation = { 0, 0, 0 };
        btQuaternion rotation = { 0, 0, 0, 1 };
        btVector3 scale = { 1, 1, 1 };
    };

    std::vector<Instance> staticInstances;
    std::vector<Instance> foliageInstances;
    std::vector<Instance> triggerInstances;

    auto mapStatic = mapValue.find("Static");
    auto mapFoliage = mapValue.find("Foliage");
    auto mapTrigger = mapValue.find("Trigger");

    static auto pushInstances = [](toml::Value * src, std::vector<Instance> & dest)
    {
        auto vec3 = [](toml::Value const* v) -> btVector3
        {
            return
            {
                (btScalar)v->find(0)->asNumber(),
                (btScalar)v->find(1)->asNumber(),
                (btScalar)v->find(2)->asNumber(),
            };
        };

        auto quat = [](toml::Value const* v) -> btQuaternion
        {
            return
            {
                (btScalar)v->find(0)->asNumber(),
                (btScalar)v->find(1)->asNumber(),
                (btScalar)v->find(2)->asNumber(),
                (btScalar)v->find(3)->asNumber(),
            };
        };

        for (auto & tInstance : src->as<toml::Array>())
        {
            Instance instance;
            instance.name = tInstance.get<std::string>("name");
            instance.model = tInstance.get<std::string>("model");

            toml::Value const* translationValue = tInstance.findChild("translation");
            toml::Value const* rotationValue = tInstance.findChild("rotation");
            toml::Value const* scaleValue = tInstance.findChild("scale");

            if (translationValue) { instance.translation = vec3(translationValue); }
            if (rotationValue) { instance.rotation = quat(rotationValue); }
            if (scaleValue) { instance.scale = vec3(scaleValue); }

            dest.push_back(instance);
        }
    };

    if (mapStatic) pushInstances(mapStatic, staticInstances);
    if (mapFoliage) pushInstances(mapFoliage, foliageInstances);
    if (mapTrigger) pushInstances(mapTrigger, triggerInstances);

    // TODO USE THIS //    
}

// Adds a pointlight to the map
void Map::add(FrameLight frameLight)
{
    m_lights.push_back(newd LightObject(frameLight));
}

//
//// Adds a static hitbox to the map
//void Map::add(FrameHitbox frameHitbox)
//{
//    m_hitboxes.push_back(newd StaticObject(
//        frameHitbox.modelID, 
//        m_physicsPtr->createBody(
//            Cube(frameHitbox.position, frameHitbox.rotation, frameHitbox.dimensions), /* Shape */             
//            NULL,                   /* Mass */              
//            false,                  /* Sensor */            
//            Physics::COL_HITBOX,    /* Collision Type */    
//            Physics::COL_EVERYTHING /* Collides With */     
//        ),
//        frameHitbox.dimensions, /* Graphical Scaling */ 
//        StaticObject::NavigationMeshFlags::NO_CULL
//    ));
//}