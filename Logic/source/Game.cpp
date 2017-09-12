#include "Game.h"

using namespace Logic;

Game::Game()
{
	physics = nullptr;
}

Game::~Game() { }

bool Game::init()
{
	bool result;

	// Initializing Bullet physics
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();	// Configuration
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);				// The default collision dispatcher
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();								// Detecting aabb-overlapping object pairs
	btSequentialImpulseConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver;	// Default constraint solver
	physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
	result = physics->init();

	return result;
}

void Game::clear()
{
	// Deleting physics
	physics->clear();
	delete physics;
}

void Game::update(float deltaTime)
{
	// Updating physics
<<<<<<< HEAD
	m_physics->update(deltaTime);

	// Updating player
	m_player->update(deltaTime);

	// Debugging for testing if physics is working:
	// printf("Player:		");  m_player->consoleWritePosition();
	// printf("Plane:		"); m_plane->consoleWritePosition();
}

void Game::render()
{
	// Clearing previous frame
	m_register.clear();

	// Drawing player
	m_player->render(m_register);
}

std::queue<Graphics::RenderInfo*>* Game::getRenderQueue()
{
	return m_register.getRenderInfo();
=======
	physics->update(deltaTime);
>>>>>>> logic
}