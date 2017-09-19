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

	// Initializing Player
	m_player = new Player(m_physics->createBody(Sphere({ 5, -15, 0 }, { 0, 0, 0 }, 4.f), 75.f, false));
	m_player->init();

	// Initializing Menu's
	m_menu = new MenuMachine();
	m_menu->initialize(gameStateGame); //change here to accses menu tests
									   
	// Initializing the map
	m_map = new Map();
	m_map->init(m_physics);

	return result;
}

void Game::clear()
{
	delete m_physics;
	delete m_player;
	m_menu->clear();
	delete m_menu;
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
		m_physics->update(deltaTime);
		m_player->update(deltaTime);

		// Updating Entities
		m_entityManager.update(deltaTime);
		
		// Updating map objects
		m_map->update(deltaTime);
	}
}

void Game::render(Graphics::Renderer& renderer)
{
	if (m_menu->currentState() != gameStateGame)
	{

	}
	else
	{
		m_player->render(renderer);
		m_map->render(renderer);

		// Drawing Entities (enemies / triggers)
		m_entityManager.render(renderer);
	}
}

DirectX::SimpleMath::Vector3 Game::getPlayerForward()
{
	return m_player->getForward();
}

DirectX::SimpleMath::Vector3 Game::getPlayerPosition()
{
	return m_player->getPosition();
}