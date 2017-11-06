#include <StateGameCampfire.h>
#include <State.h>
#include <StateBuffer.h>
#include <Graphics\include\MainCamera.h>
#include <Graphics\include\Device.h>

// Input Singletons
#include <Keyboard.h>
#include <Mouse.h>

// Debugging purposes
#include <DebugDefines.h>
#include <Engine\Typing.h>
#include <Engine\DebugWindow.h> 
#include <GameType.h>

using namespace Logic;

StateGameCampfire::StateGameCampfire(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    // Initializing Bullet physics
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();				// Configuration
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);	// The default collision dispatcher
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();								// Detecting aabb-overlapping object pairs
    btSequentialImpulseConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver();			// Default constraint solver
    m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
    m_physics->init();

    // Initializing the Map
    m_map = newd Map();
    m_map->init(m_physics, "Campfire.txt");
}

StateGameCampfire::~StateGameCampfire()
{
    delete m_physics;
    delete m_map;
}

void StateGameCampfire::reset()
{

}

void StateGameCampfire::update(float deltaTime)
{
    static DirectX::SimpleMath::Vector3 movingCameraPosition(0, 0, 0);
    static DirectX::SimpleMath::Vector3 movingCameraForward(0, 0, 1);
    movingCameraPosition.y += 0.001f * deltaTime;
    Global::mainCamera->update(movingCameraPosition, movingCameraForward, context);

    PROFILE_BEGIN("Physics");
    m_physics->update(deltaTime);
    PROFILE_END();

    PROFILE_BEGIN("Map");
    m_map->update(deltaTime);
    PROFILE_END();
}

void StateGameCampfire::render() const
{
    PROFILE_BEGIN("Render Map");
    m_map->render();
    PROFILE_END();
}