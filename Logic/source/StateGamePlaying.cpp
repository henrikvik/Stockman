#include <StateGamePlaying.h>
#include <StateBuffer.h>
#include <State.h>

// Input Singletons
#include <Keyboard.h>
#include <Mouse.h>

// Debugging purposes
#include <DebugDefines.h>
#include <Engine\Typing.h>
#include <Engine\DebugWindow.h> 
#include <GameType.h>

using namespace Logic;

// Game starting static configurations
const int StateGamePlaying::GAME_START::UNIQUE_CARDS = 13;
const btVector3 StateGamePlaying::GAME_START::PLAYER_SCALE = { 1.5f, 3.0f, 1.5f };
const btVector3 StateGamePlaying::GAME_START::PLAYER_ROTATION = { 0.0f, 0.0f, 0.0f };

StateGamePlaying::StateGamePlaying(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
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
    m_player = newd Player(Resources::Models::UnitCube, nullptr, GAME_START::PLAYER_SCALE);
    m_player->init(m_physics, m_projectileManager);
    Sound::NoiseMachine::Get().update(m_player->getListenerData());

    // Initializing Highscore Manager
    m_highScoreManager = newd HighScoreManager();
    m_highScoreManager->setName("Stockman");

    // Initializing the Map
    m_map = newd Map();
    m_map->init(m_physics, "");

    // Initializing Card Manager
    m_cardManager = newd CardManager(GAME_START::UNIQUE_CARDS);

    // Initializing Combo's
    ComboMachine::Get().ReadEnemyBoardFromFile("Nothin.");
    ComboMachine::Get().Reset();

    // Loading func
    m_entityManager.setSpawnFunctions(*m_projectileManager, *m_physics);

    // Sorry Lucas, I broke it
    //#ifdef _DEBUG
    //    DebugWindow *win = DebugWindow::getInstance();
    //    win->registerCommand("SETGAMESTATE", [&](std::vector<std::string> &para) -> std::string {
    //        try {
    //            this->m_menu->setStateToBe(static_cast<GameState> (stoi(para[0])));
    //            return "Menu State set to " + stoi(para[0]);
    //        }
    //        catch (std::exception e) {
    //            return "Chaos is a pit.";
    //        }
    //    });
    //
    //    for (int i = 1; i < args; i++) // first arg is name of file
    //        for (std::wstring const &str : GameTypeStr)
    //            if (wcscmp(str.c_str(), cmdLine[i]) == 0)
    //                m_gameType = static_cast<GameType> (i - 1); // offset for filename
    //
    //    for (std::string const &cmd : GameCommands[m_gameType])
    //        if (!cmd.empty())
    //            win->doCommand(cmd.c_str());
    //#endif

    RenderQueue::get().clearAllQueues();

    //temp? probably
    static SpecialEffectRenderInfo info;
    info.type = info.Snow;
    info.restart = true;

    RenderQueue::get().queue(&info);
}

StateGamePlaying::~StateGamePlaying()
{
    m_projectileManager->clear();

    m_entityManager.resetTriggers();
    m_entityManager.deallocateData(); // Have to deallocate before deleting physics

    delete m_physics;
    delete m_player;
    delete m_map;
    delete m_cardManager;
    delete m_highScoreManager;
    delete m_projectileManager;
}

void StateGamePlaying::reset()
{
    m_projectileManager->removeAllProjectiles();
    m_player->reset();

    m_entityManager.resetTriggers();
    m_entityManager.deallocateData();
    m_waveTimeManager.reset();

    m_cardManager->resetDeck();
    m_hudManager.reset();

    ComboMachine::Get().Reset();
}

void StateGamePlaying::update(float deltaTime)
{
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
}

void StateGamePlaying::render() const
{
    // Debug Draw physics
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift))
        m_physics->render();

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
    m_hudManager.render();
    PROFILE_END();

    m_fpsRenderer.render();
}

void StateGamePlaying::gameOver()
{
    m_highScoreManager->addNewHighScore(ComboMachine::Get().GetCurrentScore());
    reset();
}