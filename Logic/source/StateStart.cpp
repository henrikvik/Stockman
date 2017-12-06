#include <StateStart.h>
#include <StateMachine\StateBuffer.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Engine\Typing.h>
#include <DebugDefines.h>
#include <Singeltons\DebugWindow.h> 
#include <Physics\Physics.h>
#include <Map.h>
#include <Graphics\include\MainCamera.h>
#include <Graphics\include\Device.h>
#include <Misc\Test\TestingWork.h>

using namespace Logic;

StateStart::StateStart(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    Sound::NoiseMachine::Get().loadMenuSounds();
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::AMBIENT_STORM, nullptr, true);
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::MUSIC_MAIN_MENU, nullptr, true);

    // Making a small little campfire map to look at
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();			
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);	
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();								
    btSequentialImpulseConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver();			
    m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
    m_physics->init();
    m_map = newd Map();
    m_map->init(m_physics);
    m_map->loadStartMenuScene();

    // Initializing Menu's
    m_menu = newd iMenuMachine();
    m_menu->queueMenu(iMenu::MenuGroup::Intro);

    // for some tests
    TestingWork work;

    // ! Reminder !  
    // Gives a small mem leak as for right now, but it's too cool to remove ^.^
    // Initializing campfire
    m_campfire = Graphics::FXSystem->getEffect("FireSmoke");

    
}

StateStart::~StateStart()
{
    delete m_menu;
    delete m_physics;
    delete m_map;
    Sound::NoiseMachine::Get().clearCurrent();
}

void StateStart::reset() { }

void StateStart::update(float deltaTime)
{
    Graphics::FXSystem->processEffect(&m_campfire, { 0, 0, 0 }, deltaTime / 1000.f);

    //temp 
    SpecialEffectRenderInfo info;
    info.type = SpecialEffectRenderInfo::DoF;
    QueueRender(info);

    PROFILE_BEGIN("Physics");
    m_physics->update(deltaTime);
    PROFILE_END();

    PROFILE_BEGIN("Map");
    m_map->update(deltaTime);
    PROFILE_END();

    m_fpsRenderer.updateFPS(deltaTime);
    m_menu->update(deltaTime);
}

void StateStart::render() const
{
    PROFILE_BEGIN("Render Map");
    m_map->render();
    PROFILE_END();

    PROFILE_BEGIN("Render Menu's");
    m_menu->render();
    PROFILE_END();

    m_fpsRenderer.render();
}