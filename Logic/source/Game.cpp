#include "Game.h"

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

	// Initializing Bullet physics
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();	// Configuration
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);				// The default collision dispatcher
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();								// Detecting aabb-overlapping object pairs
	btSequentialImpulseConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver;	// Default constraint solver
	m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
	result = m_physics->init();

	// Making a player class with a body at 100 above origin
	m_player = new Player();
	m_player->init(m_physics, BodyDesc(1, { 0, 100, 0 }, { 1, 1, 1 }));

	// Making the map
	m_map = new Map();
	m_map->init(m_physics);

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

	// Debugging for testing if physics is working:
	// printf("Player:		");  m_player->consoleWritePosition();
}

void Game::render()
{
	// Clearing previous frame
	m_register.clear();

	// Drawing player
	m_player->render(m_register);

	// Drawing map
	m_map->render(m_register);
}

std::queue<Graphics::RenderInfo*>* Game::getRenderQueue()
{
	return m_register.getRenderInfo();
}