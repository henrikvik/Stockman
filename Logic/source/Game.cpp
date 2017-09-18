#include "Game.h"

#include <AI/EntityManager.h>
#include <thread>

using namespace Logic;

Game::Game()
{
	m_physics = nullptr;
	m_player = nullptr;
	m_map = nullptr;
}

Game::~Game() 
{	
	clear();
}

bool Game::init()
{
	bool result;

	// TESTING REMOVEREMOVEREMOVEREMOVEREMOVEREMOVEREMOVEREMOVEREMOVEREMOVEREMOVEREMOVEREMOVEREMOVE 
/*	EntityManager entity;
	entity.spawnWave();
	while (true) {
		entity.update(5.f);
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
	} */
	// TESTING REMOVE REMOVE REMOVE RMEOVE REOMVEREOMVEREOMVEREOMVEREOMVEREOMVEREOMVEREOMVEREOMVE

	// Initializing Bullet physics
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();	// Configuration
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);				// The default collision dispatcher
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();								// Detecting aabb-overlapping object pairs
	btSequentialImpulseConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver;	// Default constraint solver
	m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
	result = m_physics->init();

	m_player = m_physics->addPlayer(Cube({ 0, 5, -15 }, { 0, 0, 90 }, { 1, 1, 1 }), 100);
	m_player->init();

	// Making the map
	m_map = new Map();
	m_map->init(m_physics);

    TEST_CUBE = m_physics->addBody(Cube({ 0, 10 , 0 }, { 0, 0 , 0 }, { 0.5, 0.5 , 0.5 }), 25.f, false);
    
	return result;
}

void Game::clear()
{
	delete m_physics;
	delete m_player;
	delete m_map;
}

void Game::update(float deltaTime)
{
	// Updating physics
	m_physics->update(deltaTime);

	// Updating player
	m_player->update(deltaTime);

	// Updating map objects
	m_map->update(deltaTime);

    TEST_CUBE->update(deltaTime);

	// Debugging for testing if physics is working:
	// printf("Player:		");		m_player->consoleWritePosition();
}

void Game::render()
{
	// Clearing previous frame
	m_register.clear();
	
	// Drawing player
	m_player->render(m_register);

	// Drawing map
	m_map->render(m_register);
   
//	TEST_CUBE->render(m_register);

}

std::queue<Graphics::RenderInfo*>* Game::getRenderQueue()
{
	return m_register.getRenderInfo();
}

DirectX::SimpleMath::Vector3 Game::getPlayerForward()
{
	return m_player->getForward();
}

DirectX::SimpleMath::Vector3 Game::getPlayerPosition()
{
	return m_player->getPosition();
}
