#include "Game.h"

using namespace Logic;

Game::Game()
{
	m_physics = nullptr;
	m_player = nullptr;
	m_map = nullptr;
	m_projectileManager = nullptr;
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
	btSequentialImpulseConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver();	// Default constraint solver
	m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
	result = m_physics->init();

	// Initializing Projectile Manager
	m_projectileManager = new ProjectileManager(m_physics);

	// Initializing Player
	m_player = new Player(m_physics->createBody(Cylinder({ 5, -15, 0 }, { 0, 0, 0 }, { 0.5, 3.0, 0.5 }), 75.f, false));
	m_player->init(m_projectileManager);

	// Initializing Menu's
	m_menu = newd MenuMachine();
	m_menu->initialize(gameStateMenuMain); //change here to accses menu tests
									   
	// Initializing the map
	m_map = newd Map();
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
	m_projectileManager->clear();
	delete m_projectileManager;
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
		m_entityManager.update(deltaTime);
		m_map->update(deltaTime);

		m_projectileManager->update(deltaTime);
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
		m_entityManager.render(renderer);

		// Drawing Projectiles
		m_projectileManager->render(renderer);
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