#include "Game.h"

using namespace Logic;

Game::Game()
{
	m_physics = nullptr;
	m_player = nullptr;
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
	m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
	result = m_physics->init();

	// Making a player class with a body at 100 above origin
	m_player = new Player();
	m_player->init(m_physics, BodyDesc(1, { 0, 100, 0 }, { 1, 1, 1 }));

	// Making an infite plane on origin
	m_plane = new Hitbox();
	m_plane->init(m_physics, BodyDesc({ 0, 0, 0 }, 1, { 0, 1, 0 }));

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

	// Deleting hitbox
	m_plane->clear();
	delete m_plane;
}

void Game::update(float deltaTime)
{
	// Updating physics
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
>>>>>>> logic
}