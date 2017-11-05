#include "Map.h"
#include <Keyboard.h>
#include <Graphics\include\Structs.h>
#include <Graphics\include\Utility\DebugDraw.h>

using namespace Logic;

Map::Map() { }

Map::~Map() 
{
	clear();
}

void Map::add(FrameLight frameLight)
{
    m_lights.push_back(new LightObject(frameLight));
}

void Map::add(FrameProp frameProp)
{
  //  m_objects.push_back(new Object(frameProp.modelID, frameProp.position, frameProp.rotation));
}

void Map::add(FrameHitbox frameHitbox)
{
    if (frameHitbox.modelID == Graphics::GROUND)
        m_hitboxes.push_back(new StaticObject(frameHitbox.modelID, m_physicsPtr->createBody(
            Cube(frameHitbox.position, frameHitbox.rotation, frameHitbox.dimensions), NULL, false,
            Physics::COL_HITBOX,
            Physics::COL_EVERYTHING),
            {1, 1.f, 1},
            StaticObject::NavigationMeshFlags::CULL
        ));
    else
        m_hitboxes.push_back(new StaticObject(frameHitbox.modelID, m_physicsPtr->createBody(
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
    m_drawHitboxesAndLights = false;

    readFromFile("maya.level");
	
    debugInitProps();
    debugInitObjects();
}

// This could be loaded from level file
void Map::readFromFile(std::string path)
{
    // Loads hitboxes
    std::vector<FrameHitbox> hitboxes;
    hitboxes.push_back({ { 0, -10, 0 }, {0, 0, 0}, {500.f, 10, 500.f}, Graphics::GROUND });
    hitboxes.push_back({ { 60, 0.75, 60 },{ 0, 0, 0 },{ 45, 0.75, 45 }, Graphics::CUBE });
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
    hitboxes.push_back({ { -60, 1, -60 },{ 0, 0.3f, 0 },{ 25, 3, 25 }, Graphics::CUBE });

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

void Map::clear()
{
	for (size_t i = 0; i < m_props.size(); i++)     delete m_props[i];
	for (size_t i = 0; i < m_hitboxes.size(); i++)  delete m_hitboxes[i];
	for (size_t i = 0; i < m_objects.size(); i++)   delete m_objects[i];
    for (size_t i = 0; i < m_lights.size(); i++)    delete m_lights[i];

	m_props.clear();
	m_hitboxes.clear();
	m_objects.clear();
    m_lights.clear();
}

void Map::update(float deltaTime)
{
    // If user holds tab, draw debug info
    m_drawHitboxesAndLights = DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift) ? true : false;

	// Updating interactable objects
	for (size_t i = 0; i < m_objects.size(); i++)
		m_objects[i]->update(deltaTime);
}

void Map::render(Graphics::Renderer& renderer)
{
	for (Object* o : m_props)           o->render(renderer);
	for (Speaker* e : m_objects)        e->render(renderer);
    for (LightObject* l : m_lights)     l->render(renderer);
    for (StaticObject* e : m_hitboxes)  e->render(renderer); // Should not be visiable at all in release

	// Drawing hitboxes & lights
    if (m_drawHitboxesAndLights)
    {
        for (StaticObject* e : m_hitboxes)
            e->renderD(renderer);
        for (LightObject* l : m_lights) {
            //l->renderD(renderer);
            Graphics::Debug::PointLight(*l);
        }
    }
}
	
std::vector<Object*>*				Map::getProps()				{ return &m_props;				}
std::vector<StaticObject*>*			Map::getHitboxes()			{ return &m_hitboxes;			}
std::vector<Speaker*>*				Map::getObjects()			{ return &m_objects;			}
std::vector<LightObject*>*			Map::getLights()            { return &m_lights;             }

void Map::debugInitProps()
{

}

void Map::debugInitObjects()
{
    // Debugging for Sound bug testing
    btVector3 halfextent(1.0, 1.0, 1.0);
    Speaker* box = new Speaker(m_physicsPtr->createBody(Cube({ -25, 3, 75 }, { 0, 0, 0 }, halfextent), 1.f, false), halfextent, Graphics::CUBE);
    box->getSoundSource()->autoPlaySFX(Sound::SFX::BOING, 6000.f, 250.f);
    m_objects.push_back(box);

    box = new Speaker(m_physicsPtr->createBody(Cube({ -26, 3, 75 }, { 0, 0, 0 }, halfextent), 1.f, false), halfextent, Graphics::CUBE);
    box->getSoundSource()->delayPlayMusic(Sound::MUSIC::TEST_MUSIC, 500.f);
    m_objects.push_back(box);

    box = new Speaker(m_physicsPtr->createBody(Cube({ -23, 3, 74 }, { 0, 0, 0 }, halfextent), 1.f, false), halfextent, Graphics::CUBE);
    box->getSoundSource()->autoPlaySFX(Sound::SFX::BOING, 4000.f, 250.f);
    m_objects.push_back(box);

    box = new Speaker(m_physicsPtr->createBody(Cube({ -23, 2, 73 }, { 0, 0, 0 }, halfextent), 1.f, false), halfextent, Graphics::CUBE);
    box->getSoundSource()->autoPlaySFX(Sound::SFX::BOING, 3500.f, 250.f);
    m_objects.push_back(box);
}