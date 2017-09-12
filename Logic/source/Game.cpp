#include "Game.h"

using namespace Logic;

Game::Game()
{
	m_physics = nullptr;
	m_player = nullptr;
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

	m_player = new Player();
	m_player->init(m_physics, RigidBodyDesc(ShapeRectangle, 1, { 0, 0, 0 }, { 1, 1, 1 } ));

	m_menu = new MenuMachine();
	m_menu->initialize();
	m_menu->showMenu(gameStateMenuMain);
	return result;
}

void Game::clear()
{
	// Deleting physics
	m_physics->clear();
	delete m_physics;

	// Deleting player
	m_player->clear();
	delete m_player;

	m_menu->clear();
	delete m_menu;
}

void Game::update(float deltaTime)
{
	// Updating physics
	m_physics->update(deltaTime);

	// Updating player
	m_player->update(deltaTime);
//	m_player->consoleWritePosition();

	m_menu->update();
}