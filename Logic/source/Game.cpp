#include "Game.h"
#include <iostream>
#include <Engine\Typing.h>

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
	Typing::releaseInstance();
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

	m_highScoreManager = newd HighScoreManager();

	std:string name = "Stockman";

	m_highScoreManager->setName(name);

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
	m_cardManager->createDeck(NUMBER_OF_EACH_CARD);

	// Initializing Combo's
	ComboMachine::Get().ReadEnemyBoardFromFile("Nothin.");
	ComboMachine::Get().Reset();

	// Initializing Sound
	NoiseMachine::Get().init();

	// Initializing Menu's
	m_menu = newd MenuMachine(m_highScoreManager->getName());
	m_menu->initialize(STARTING_STATE);
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
	NoiseMachine::Get().clear();
}

void Game::reset()
{
	/*m_entityManager.clear();*/
	m_player->reset();
	
	ComboMachine::Get().Reset();
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
	Card temp;
	switch (m_menu->currentState())
	{
	case gameStateGame:
		if (m_menu->getStateToBe() == GameState::gameStateGameUpgrade)
		{
			m_cardManager->pickThree(false); //get some trigger for injury
			m_menu->update(m_gameTime.dt);
			break;
		}
		else if (m_menu->getStateToBe() != GameState::gameStateDefault)
		{
			m_menu->update(m_gameTime.dt);
			break;
		}
		else
		{
			DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
			DirectX::Mouse::Get().SetMode(ks.IsKeyDown(DirectX::Keyboard::LeftAlt) ? DirectX::Mouse::MODE_ABSOLUTE : DirectX::Mouse::MODE_RELATIVE); // !TEMP!
			gameRunTime(deltaTime);
		}

		break;
	case gameStateGameUpgrade:
		m_menu->update(m_gameTime.dt);
		if (m_menu->getStateToBe() != GameState::gameStateDefault)
		{
			break;
		}
		else
		{
			/* Temp sound testing */
			if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::J)) NoiseMachine::Get().playSFX(SFX::BOING);
			if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::K)) NoiseMachine::Get().playMusic(MUSIC::NES);
			if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::L)) NoiseMachine::Get().setGroupVolume(CHANNEL_GROUP::CHANNEL_MASTER, 0);
		}
		temp = m_cardManager->pick(m_menu->getChoiceUpgrade());

		if (temp.getName().compare("") != 0 && temp.getDescription().compare("") != 0)
		{
			//add information to player
			m_menu->setStateToBe(gameStateMenuMain); //change to gameStateGame

			for (auto const& ID : temp.getUpgradesID())
			{
				if (temp.getIsEffect())
				{
					m_player->getStatusManager().addStatus(static_cast<StatusManager::EFFECT_ID>(ID), 1); //edit to how you feel it should be
				}
				else
				{
					m_player->getStatusManager().addUpgrade(static_cast<StatusManager::UPGRADE_ID>(ID));
				}
			}
		}

		gameRunTime(deltaTime);

		break;
	case gameStateLoading:
		m_menu->update(m_gameTime.dt);
		break;
	case gameStateMenuMain:
		if (m_menu->getStateToBe() == GameState::gameStateGameUpgrade) //remove once duebugged
		{
			m_cardManager->pickThree(false); //get some trigger for injury
		}
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

void Game::gameRunTime(float deltaTime)
{
	ComboMachine::Get().Update(deltaTime);
	waveUpdater();

	PROFILE_BEGIN("Sound");
	NoiseMachine::Get().update(m_player->getListenerData());
	PROFILE_END();

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

	if (m_player->getHP() <= 0)
		gameOver();
}

void Game::gameOver()
{
	printf("You ded bro.\n");
	m_highScoreManager->addNewHighScore(ComboMachine::Get().GetCurrentScore());
	m_menu->setStateToBe(GameState::gameStateGameOver);

	for (int i = 0; i < 10; i++)
	{
		if (m_highScoreManager->gethighScore(i).score != -1)
		{
			highScore[i] = m_highScoreManager->gethighScore(i).name + ": " + std::to_string(ComboMachine::Get().GetCurrentScore());
			break;
		}
	}
	reset();
}

void Game::render(Graphics::Renderer& renderer)
{
	switch (m_menu->currentState())
	{
	case gameStateGame:
		gameRunTimeRender(renderer);
		// Debug Draw physics
		if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift))
			m_physics->render(renderer);
		break;

	case gameStateGameUpgrade:
		gameRunTimeRender(renderer);
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

void Game::gameRunTimeRender(Graphics::Renderer& renderer)
{
	PROFILE_BEGIN("Player Render");
	m_player->render(renderer);
	PROFILE_END();

	PROFILE_BEGIN("Render Map");
	m_map->render(renderer);
	PROFILE_END();

	PROFILE_BEGIN("Render Enemies & Triggers");
	m_entityManager.render(renderer);
	PROFILE_END();

	PROFILE_BEGIN("Render Projectiles");
	m_projectileManager->render(renderer);
	PROFILE_END();
}

void Logic::Game::menuRender(Graphics::Renderer * renderer)
{
	m_menu->render(*renderer);
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
