#include "Game.h"
#include <iostream>
#include <Engine\Typing.h>
#include <DebugDefines.h>

using namespace Logic;

const int Game::NUMBER_OF_UNIQUE_CARDS      = 13;

const GameState Game::STATE_START           = gameStateMenuMain;
const btVector3 Game::PLAYER_START_SCALE    = { 1.5f, 3.0f, 1.5f };
const btVector3 Game::PLAYER_START_ROTATION = { 0.0f, 0.0f, 0.0f };

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
	// Initializing Sound
	Sound::NoiseMachine::Get().init();

	// Initializing Bullet physics
	btDefaultCollisionConfiguration* collisionConfiguration		= new btDefaultCollisionConfiguration();				// Configuration
	btCollisionDispatcher* dispatcher							= new btCollisionDispatcher(collisionConfiguration);	// The default collision dispatcher
	btBroadphaseInterface* overlappingPairCache					= new btDbvtBroadphase();								// Detecting aabb-overlapping object pairs
	btSequentialImpulseConstraintSolver* constraintSolver		= new btSequentialImpulseConstraintSolver();			// Default constraint solver
	m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
	m_physics->init();

	// Initializing Projectile Manager
	m_projectileManager = newd ProjectileManager(m_physics);

	// Initializing Player
	m_player = new Player(Resources::Models::UnitCube, nullptr, Game::PLAYER_START_SCALE);
	m_player->init(m_physics, m_projectileManager);
	Sound::NoiseMachine::Get().update(m_player->getListenerData());

	// Initializing Highscore Manager
	m_highScoreManager = newd HighScoreManager();
	m_highScoreManager->setName("Stockman");

	// Initializing Menu's
	m_menu = newd MenuMachine(m_highScoreManager->getName());
	m_menu->initialize(Game::STATE_START);

	// Initializing the Map
	m_map = newd Map();
	m_map->init(m_physics);

	// Initializing Card Manager
	m_cardManager = newd CardManager();
	m_cardManager->init();
	m_cardManager->createDeck(Game::NUMBER_OF_UNIQUE_CARDS);

	// Initializing Combo's
	ComboMachine::Get().ReadEnemyBoardFromFile("Nothin.");
	ComboMachine::Get().Reset();


    // Loading func
    m_entityManager.setSpawnFunctions(*m_projectileManager, *m_physics);

	/*int m_currentHP;
	DebugWindow *debugWindow = DebugWindow::getInstance();
	debugWindow->registerCommand("SPAWNENEMIES", [&](std::stringstream &args)->std::string
	{
		int enemyCount;
		float enemyHP;
		args.read((char*)enemyCount, sizeof(int));
		args.read((char*)enemyHP, sizeof(float));
	});


	---- SPAWNENEMIES 100 10.0f */

}

void Game::clear()
{
	m_menu->clear();
	m_projectileManager->clear();
	Sound::NoiseMachine::Get().clear();

	delete m_physics;
	delete m_player;
	delete m_menu;
	delete m_map;
	delete m_cardManager;
	delete m_highScoreManager;
	delete m_projectileManager;
}

void Game::reset()
{
	/*m_entityManager.clear();*/
	m_player->reset();
	
	ComboMachine::Get().Reset();
}

void Game::update(float deltaTime)
{
	m_gameTime.update(deltaTime);
    m_fpsRenderer.updateFPS(deltaTime);
	Card temp; // per frame allocation for something that is just a copy of an already existing ob

	// Handles slow-mo & speed-up
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
			if (ks.IsKeyDown(DirectX::Keyboard::U))
			{
				m_menu->setStateToBe(gameStateGameUpgrade);
				m_cardManager->pickThree(false);
			}
		}

		break;
	case gameStateGameUpgrade:
		m_menu->update(m_gameTime.dt);
		if (m_menu->getStateToBe() != GameState::gameStateDefault)
		{
			break;
		}
		temp = m_cardManager->pick(m_menu->getChoiceUpgrade());

		if (temp.getName().compare("") != 0 && temp.getDescription().compare("") != 0)
		{
			//add information to player
			m_menu->setStateToBe(gameStateGame); //change to gameStateGame

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
	m_waveTimeManager.update(deltaTime, m_entityManager);

	PROFILE_BEGIN("Sound");
	Sound::NoiseMachine::Get().update(m_player->getListenerData());
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

    PROFILE_BEGIN("HUD");
    m_hudManager.update(*m_player, m_waveTimeManager, m_entityManager);
    PROFILE_END();

	if (m_player->getHP() <= 0)
		gameOver();
}

void Game::gameOver()
{
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

void Game::render()
{
	switch (m_menu->currentState())
	{
	case gameStateGame:
		gameRunTimeRender();
		// Debug Draw physics
		if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift))
			m_physics->render();
		break;

	case gameStateGameUpgrade:
		gameRunTimeRender();
		break;

	case gameStateLoading:
	case gameStateMenuMain:
	case gameStateMenuSettings:
	case gameStateGameOver:
		/*m_menu->render(renderer);*/
	default:  m_menu->render();
		break;
	}

    m_fpsRenderer.render();
}

void Game::gameRunTimeRender()
{
	PROFILE_BEGIN("Player Render");
	m_player->render();
	PROFILE_END();

	PROFILE_BEGIN("Render Map");
	m_map->render();
	PROFILE_END();

	PROFILE_BEGIN("Render Enemies & Triggers");
	m_entityManager.render();
	PROFILE_END();

	PROFILE_BEGIN("Render Projectiles");
	m_projectileManager->render();
	PROFILE_END();

    PROFILE_BEGIN("Render HUD");
    //m_hudManager.render();
    PROFILE_END();
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
