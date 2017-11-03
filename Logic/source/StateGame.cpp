#include <StateGame.h>

// Input Singletons
#include <Keyboard.h>
#include <Mouse.h>

// Debugging purposes
#include <DebugDefines.h>
#include <Engine\Typing.h>
#include <Engine\DebugWindow.h> 
#include <GameType.h>

#include <Graphics\include\Renderer.h> // Remove this when merged with henke's

using namespace Logic;

// Game starting static configurations
const int StateGame::GAME_START::UNIQUE_CARDS               = 13;
const btVector3 StateGame::GAME_START::PLAYER_SCALE         = { 1.5f, 3.0f, 1.5f };
const btVector3 StateGame::GAME_START::PLAYER_ROTATION      = { 0.0f, 0.0f, 0.0f };

StateGame::StateGame()
{
    // Initializing Sound
    Sound::NoiseMachine::Get().init();

    // Initializing Bullet physics
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();				// Configuration
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);	// The default collision dispatcher
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();								// Detecting aabb-overlapping object pairs
    btSequentialImpulseConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver();			// Default constraint solver
    m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
    m_physics->init();

    // Initializing Projectile Manager
    m_projectileManager = newd ProjectileManager(m_physics);

    // Initializing Player
    m_player = newd Player(Graphics::ModelID::CUBE, nullptr, GAME_START::PLAYER_SCALE);
    m_player->init(m_physics, m_projectileManager);
    Sound::NoiseMachine::Get().update(m_player->getListenerData());

    // Initializing Highscore Manager
    m_highScoreManager = newd HighScoreManager();
    m_highScoreManager->setName("Stockman");

    // Initializing the Map
    m_map = newd Map();
    m_map->init(m_physics);

    // Initializing Card Manager
    m_cardManager = newd CardManager();
    m_cardManager->init();
    m_cardManager->createDeck(GAME_START::UNIQUE_CARDS);

    // Initializing Combo's
    ComboMachine::Get().ReadEnemyBoardFromFile("Nothin.");
    ComboMachine::Get().Reset();

    // Loading func
    m_entityManager.setSpawnFunctions(*m_projectileManager, *m_physics);
}

StateGame::~StateGame()
{
    m_projectileManager->clear();
    m_entityManager.deallocateData(); // Have to deallocate before deleting physics
    Sound::NoiseMachine::Get().clear();

    delete m_physics;
    delete m_player;
    delete m_map;
    delete m_cardManager;
    delete m_highScoreManager;
    delete m_projectileManager;
}

void StateGame::reset()
{
    m_entityManager.deallocateData();
    m_player->reset();
    m_projectileManager->removeAllProjectiles();

    ComboMachine::Get().Reset();
}

void StateGame::update(float deltaTime)
{
    DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);

    m_fpsRenderer.updateFPS(deltaTime);
    ComboMachine::Get().Update(deltaTime);
    m_waveTimeManager.update(deltaTime, m_entityManager);

    PROFILE_BEGIN("Sound");
    Sound::NoiseMachine::Get().update(m_player->getListenerData());
    PROFILE_END();

    PROFILE_BEGIN("Player");
    m_player->updateSpecific(deltaTime);
    PROFILE_END();

    PROFILE_BEGIN("Physics");
    m_physics->update(deltaTime);
    PROFILE_END();

    PROFILE_BEGIN("AI & Triggers");
    m_entityManager.update(*m_player, deltaTime);
    PROFILE_END();

    PROFILE_BEGIN("Map");
    m_map->update(deltaTime);
    PROFILE_END();

    PROFILE_BEGIN("Projectiles");
    m_projectileManager->update(deltaTime);
    PROFILE_END();

    PROFILE_BEGIN("HUD");
    m_hudManager.update(*m_player, m_waveTimeManager, m_entityManager);
    PROFILE_END();

    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::NumPad8))
        m_player->takeDamage(1, 0);

    if (m_player->getHP() <= 0)
        gameOver();

    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::F1))
    {
        SetState(StateType::Start);
        return;
    }
}

void StateGame::render(Graphics::Renderer& renderer)
{
    // Debug Draw physics
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift))
        m_physics->render(renderer);
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::CapsLock))
        renderer.startShake(50.f, 2500.f);

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

    PROFILE_BEGIN("Render HUD");
    m_hudManager.render(renderer);
    PROFILE_END();

    m_fpsRenderer.renderFPS(renderer);
}

void StateGame::gameOver()
{
    m_highScoreManager->addNewHighScore(ComboMachine::Get().GetCurrentScore());
    reset();
}

DirectX::SimpleMath::Vector3 StateGame::getCameraForward()
{
    return m_player->getForward();
}

DirectX::SimpleMath::Vector3 StateGame::getCameraPosition()
{
    return m_player->getPosition();
}