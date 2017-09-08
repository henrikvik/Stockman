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
	physics->update(deltaTime);
}