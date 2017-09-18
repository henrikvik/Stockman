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

	// Initializing Player
	m_player = new Player(m_physics->addPlayer(Cube({ 0, 5, -15 }, { 0, 0, 90 }, { 1, 1, 1 }), 100));
	m_player->init();

	m_menu = new MenuMachine();
	m_menu->initialize(gameStateGame); //change here to accses menu tests
	// Making the map
	m_map = new Map();
	m_map->init(m_physics);

	return result;
}

void Game::clear()
{
	delete m_physics;
	delete m_player;

	// Deleting menu
	m_menu->clear();
	delete m_menu;

	// Deleting map
	delete m_map;
}

void Game::update(float deltaTime)
{
	if (m_menu->currentState() != gameStateGame)
	{
		m_menu->update();
	}
	else
	{
		// Updating physics
		m_physics->update(deltaTime);

		// Updating player
		m_player->update(deltaTime);

		// Updating map objects
		m_map->update(deltaTime);
	}
}

void Game::render()
{
	if (m_menu->currentState() != gameStateGame)
	{

	}
	else
	{
		// Clearing previous frame
		m_register.clear();

		// Drawing player
		m_player->render(m_register);

		// Drawing map
		m_map->render(m_register);
	}
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