#include "Game.h"
#include <iostream>
#define _GOD_MODE

using namespace Logic;

Game::Game()
{
	m_physics			= nullptr;
	m_player			= nullptr;
	m_map				= nullptr;
	m_projectileManager = nullptr;
	m_cardManager		= nullptr;
	m_menu				= nullptr;
	m_highScoreManager	= nullptr;
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
	m_player = new Player(Graphics::ModelID::CUBE, m_physics->createBody(Cylinder(Player::startPosition, PLAYER_START_ROT, PLAYER_START_SCA), 75.f), PLAYER_START_SCA);
	m_player->init(m_physics, m_projectileManager, &m_gameTime);

	// Initializing Menu's
	m_menu = newd MenuMachine();
	m_menu->initialize(STARTING_STATE); 

	// Initializing the map
	m_map = newd Map();
	m_map->init(m_physics);

	// Load these from a file at a later dates
	m_waveTimer		= NULL;
	m_waveCurrent	= WAVE_START;
	m_waveTime[0]	= WAVE_1_TIME;
	m_waveTime[1]	= WAVE_2_TIME;
	m_waveTime[2]	= WAVE_3_TIME;
	m_waveTime[3]	= WAVE_4_TIME;
	m_waveTime[4]	= WAVE_5_TIME;

	// Initializing Card Manager
	m_cardManager = newd CardManager();
	m_cardManager->init();

	m_highScoreManager = newd HighScoreManager();

	std:string name = "";
	//std::cout << "Enter your player name (will be used for highscore): ";
	//getline(std::cin, name);
	if (name.empty())
	{
		name = "Stockman";
	}

	m_highScoreManager->setName(name);
}

void Game::clear()
{
	delete m_physics;
	delete m_player;
	m_menu->clear();
	delete m_menu;
	delete m_map;
	delete m_cardManager;
	delete m_highScoreManager;
	m_projectileManager->clear();
	delete m_projectileManager;
}

void Logic::Game::reset()
{
	/*m_entityManager.clear();*/
	m_player->reset();
	m_waveTimer = NULL;
	m_waveCurrent = WAVE_START;
}

// Keeps check on which wave the game is on, and spawns incoming waves
void Game::waveUpdater()
{
	static bool	end = false;
	if (!end)
	{
		m_waveTimer += m_gameTime.dt;
		if (m_waveTimer > m_waveTime[m_waveCurrent])
		{
			// Spawning next wave
			m_waveCurrent++;
			printf("Spawing wave: %d\n", m_waveCurrent);
			m_entityManager.setCurrentWave(m_waveCurrent);
			m_entityManager.spawnWave(*m_physics, m_projectileManager);

			// If the player have completed all the waves
			if (m_waveCurrent == MAX_WAVES)
			{
				printf("No more waves.");
				end = true;
			}
		}
        m_player->updateWaveInfo(m_waveCurrent + 1, m_entityManager.getEnemiesAlive(), (float)((m_waveTime[m_waveCurrent] - m_waveTimer) * 0.001));
	}
}

void Game::update(float deltaTime)
{
	// Handles slow-mo & speed-up
	m_gameTime.update(deltaTime);

	switch (m_menu->currentState())
	{
	case gameStateGame:
		if (m_menu->getStateToBe() != GameState::gameStateDefault)
		{
			PROFILE_BEGIN("Menu");
			m_menu->update(m_gameTime.dt);
			PROFILE_END();
		}
		else
		{
			waveUpdater();

			PROFILE_BEGIN("Player");
			m_player->updateSpecific(m_gameTime.dt);
			PROFILE_END();

			PROFILE_BEGIN("Physics");
			m_physics->update(m_gameTime);
			PROFILE_END();

			PROFILE_BEGIN("AI & Triggers");
			m_entityManager.update(*m_player, m_gameTime.dt);
			PROFILE_END();

			PROFILE_BEGIN("Map");
			m_map->update(m_gameTime.dt);
			PROFILE_END();

			PROFILE_BEGIN("Projectiles");
			m_projectileManager->update(m_gameTime.dt);
			PROFILE_END();
		}

		if (m_player->getHP() <= 0 && m_menu->currentState() == GameState::gameStateGame)
		{
			printf("You ded bro.\n");
			m_highScoreManager->addNewHighScore();
			m_menu->setStateToBe(GameState::gameStateGameOver);

			for(int i = 0; i < 10; i++)
			{
				if (m_highScoreManager->gethighScore(i).score != -1)
				{
					highScore[i] = m_highScoreManager->gethighScore(i).name + ": " + std::to_string(m_highScoreManager->gethighScore(i).score);
				}
				else
				{
					break;
				}
			}
			reset();
		}

		break;

	case gameStateLoading:
		m_menu->update(m_gameTime.dt);
		break;
	case gameStateMenuMain:
		m_menu->update(m_gameTime.dt);
		break;
	case gameStateMenuSettings:
		m_menu->update(m_gameTime.dt);
		break;
	case gameStateGameOver:
		//Add special triggers to show the scores on the side
		m_menu->update(m_gameTime.dt);
		break;
	}
}

void Game::render(Graphics::Renderer& renderer)
{
	switch (m_menu->currentState())
	{
	case gameStateGame:

		m_player->render(renderer);

		PROFILE_BEGIN("Render Map");
		m_map->render(renderer);
		PROFILE_END();

		PROFILE_BEGIN("Render Enemies & Triggers")
		m_entityManager.render(renderer);
		PROFILE_END();

		PROFILE_BEGIN("Render Projectiles")
		m_projectileManager->render(renderer);
		PROFILE_END();

	// Debug Draw physics
	if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift))
		m_physics->render(renderer);
		break;

	case gameStateLoading:
	case gameStateMenuMain:
	case gameStateMenuSettings:
	case gameStateGameOver:
		/*m_menu->render(renderer);*/
	default:  m_menu->render(renderer);
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

int Logic::Game::getState() const
{
    return m_menu->currentState();
}
