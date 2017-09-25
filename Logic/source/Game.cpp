#include "Game.h"

using namespace Logic;

Game::Game()
{
	m_physics			= nullptr;
	m_player			= nullptr;
	m_map				= nullptr;
	m_projectileManager = nullptr;
}

Game::~Game() 
{ 
	clear();
}

void Game::init()
{
	// Initializing Bullet physics
	btDefaultCollisionConfiguration* collisionConfiguration		= new btDefaultCollisionConfiguration();				// Configuration
	btCollisionDispatcher* dispatcher							= new btCollisionDispatcher(collisionConfiguration);	// The default collision dispatcher
	btBroadphaseInterface* overlappingPairCache					= new btDbvtBroadphase();								// Detecting aabb-overlapping object pairs
	btSequentialImpulseConstraintSolver* constraintSolver		= new btSequentialImpulseConstraintSolver();			// Default constraint solver
	m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
	m_physics->init();

	// Initializing Projectile Manager
	m_projectileManager = new ProjectileManager(m_physics);

	// Initializing Player
	m_player = new Player(m_physics->createBody(Cylinder(PLAYER_START_POS, PLAYER_START_ROT, PLAYER_START_SCA), 75.f), PLAYER_START_SCA);
	m_player->init(m_projectileManager);

	// Initializing Menu's
	m_menu = newd MenuMachine();
	m_menu->initialize(STARTING_STATE); 

	// Initializing the map
	m_map = newd Map();
	m_map->init(m_physics);

	// TEST REMOVE
	m_entityManager.spawnWave(*m_physics);

	m_cardManager = newd CardManager();
	m_cardManager->init();

	//testing
	m_cardManager->createDeck(5);
	m_cardManager->shuffle(1);
	m_cardManager->pickThree(true);
	Card card = m_cardManager->pick(2);
	/*m_player->getStatusManager()->addUpgrade(card.getUpgradesID());*/
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
	// Handles slow-mo & speed-up
	m_gameTime.update(deltaTime);

	switch (m_menu->currentState())
	{
	case gameStateGame:
		m_physics->update(m_gameTime.dt);
		m_entityManager.update(*m_player, m_gameTime.dt);
		m_player->update(m_gameTime.dt);
		m_map->update(m_gameTime.dt);
		m_projectileManager->update(m_gameTime.dt);
		break;

	case gameStateLoading:
	case gameStateMenuMain:
	case gameStateMenuSettings:
	default: m_menu->update();
		break;
	}
}

void Game::render(Graphics::Renderer& renderer)
{
	switch (m_menu->currentState())
	{
	case gameStateGame:
		m_player->render(renderer);
		m_map->render(renderer);
		m_entityManager.render(renderer);
		m_projectileManager->render(renderer);
		break;

	case gameStateLoading:
	case gameStateMenuMain:
	case gameStateMenuSettings:
	default: // m_menu->render(renderer);
		break;
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