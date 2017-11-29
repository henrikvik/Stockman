#include "Map.h"
#include <Physics/Physics.h>        
#include <Keyboard.h>
#include <Graphics\include\Structs.h>
#include <Graphics\include\Utility\DebugDraw.h>
#include <Misc\RandomGenerator.h>
#include <toml\toml.h>
#include <fstream>
#include <Misc\Sound\SoundSource.h>

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
        m_hitboxes.push_back(newd StaticObject(frameHitbox.modelID, m_physicsPtr->createBody(
            Cube(frameHitbox.position, frameHitbox.rotation, frameHitbox.dimensions), NULL, false,
            Physics::COL_HITBOX,
            Physics::COL_EVERYTHING),
            frameHitbox.dimensions,
            StaticObject::NavigationMeshFlags::NO_CULL
        ));
}

void Map::init(Physics* physics)
{
    m_physicsPtr = physics;

    // Disables debug draw as default
    m_drawDebug = false;
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
#ifdef _DEBUG
    m_drawDebug = DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift) ? true : false;
#endif // _DEBUG
}

void Map::render() const
{
	for (StaticObject* o : m_props)     o->render();
    for (LightObject* l : m_lights)     l->render();
    for (StaticObject* e : m_hitboxes)  e->render(); // Hitboxes should not be visiable at all at release
}
	
std::vector<StaticObject*>*			Map::getProps()				{ return &m_props;				}
std::vector<StaticObject*>*			Map::getHitboxes()			{ return &m_hitboxes;			}
std::vector<LightObject*>*			Map::getLights()            { return &m_lights;             }

void Map::loadStartMenuScene()
{
    // Temp campfire map, remove this when an actual campfire is done
    std::vector<FrameHitbox> hitboxes;
    std::vector<FrameLight> lights;

    hitboxes.push_back({ { 0, 0.0f, 0 },{ 0, 0, 0 },{ 1.f, 1.f, 1.f },    Resources::Models::MenuScene });

    add(FrameLight({ 0.f, 0.f, 0.f }, {1.f, 0.5f, 0.3f}, 1.f, 10.f));
    
    Sound::SoundSource campfire;
    campfire.pos = { 0, 0, 0 };
    campfire.vel = { 0, 0, 0 };
    campfire.playSFX(Sound::SFX::CAMPFIRE);

    for (size_t i = hitboxes.size(); i--;) add(hitboxes[i]); for (size_t i = lights.size(); i--;) add(lights[i]);
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

    for (auto & instance : staticInstances)
    {
        if (instance.model == "Stone_Decoration") continue; // edge case

        if (strcmp(instance.model.c_str(), "Island") == 0)
        {
            btRigidBody *rb = m_physicsPtr->createBody(
                Cube(instance.translation, btVector3(), {450, 1, 450}),
                0.f, false,
                Physics::COL_HITBOX,
                Physics::COL_EVERYTHING
            );
            rb->getWorldTransform().setRotation(instance.rotation);
            m_hitboxes.push_back(newd StaticObject(
                Resources::Models::Island, 
                rb,
                instance.scale,
                StaticObject::NavigationMeshFlags::CULL
            ));
        }
        else
        {
            btRigidBody *rb = m_physicsPtr->createBody(Cube(instance.translation, btVector3(), instance.scale), 0.f, false, Physics::COL_HITBOX, Physics::COL_EVERYTHING);
            rb->getWorldTransform().setRotation(instance.rotation);
            m_hitboxes.push_back(newd StaticObject(
                Resources::Models::UnitCube, 
                rb,
                instance.scale,
                StaticObject::NavigationMeshFlags::NO_CULL
            ));
        }
    }
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