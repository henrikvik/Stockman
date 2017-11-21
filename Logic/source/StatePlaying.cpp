#include <StatePlaying.h>
#include <StateMachine\StateBuffer.h>
#include <State.h>
#include "..\include\Misc\GUI\iMenuCards.h"

#include <Misc\CommandsFile.h>

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
const int StatePlaying::GAME_START::UNIQUE_CARDS = 13;
const btVector3 StatePlaying::GAME_START::PLAYER_SCALE = { 1.5f, 3.0f, 1.5f };
const btVector3 StatePlaying::GAME_START::PLAYER_ROTATION = { 0.0f, 0.0f, 0.0f };

StatePlaying::StatePlaying(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    // Starting in game-sounds
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::AMBIENT_STORM, nullptr, true);
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::MUSIC_IN_GAME, nullptr, true);
    Sound::NoiseMachine::Get().playSFX(Sound::SFX::START_GAME, nullptr, true);

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
    m_map->init(m_physics);
    m_map->loadMap(Resources::Maps::Test_Map);

    // Initializing Card Manager
    m_cardManager = newd CardManager(GAME_START::UNIQUE_CARDS);

    // Initializing Combo's
    ComboMachine::Get().reset();

    // Initializing Menu's
    m_menu = newd iMenuMachine();
    m_menu->queueMenu(iMenu::MenuGroup::Skill);

    // Loading func
    m_entityManager.setSpawnFunctions(*m_projectileManager, *m_physics);

    CommandsFile().doCommandsFromFile();
    RenderQueue::get().clearAllQueues();

    //temp? probably
    static SpecialEffectRenderInfo info;
    info.type = info.Snow;
    info.restart = true;

    QueueRender(info);
}

StatePlaying::~StatePlaying()
{
    m_projectileManager->clear();

    m_entityManager.resetTriggers();
    m_entityManager.deallocateData(); // Have to deallocate before deleting physics

    delete m_menu;
    delete m_physics;
    delete m_player;
    delete m_map;
    delete m_cardManager;
    delete m_highScoreManager;
    delete m_projectileManager;
}

void StatePlaying::reset()
{
    m_projectileManager->removeAllProjectiles();
    m_player->reset();

    m_entityManager.resetTriggers();
    m_entityManager.deallocateData();
    m_waveTimeManager.reset();

    m_cardManager->resetDeck();
    m_hudManager.reset();

    ComboMachine::Get().reset();
}

void StatePlaying::update(float deltaTime)
{
    m_fpsRenderer.updateFPS(deltaTime);
   
    PROFILE_BEGIN("In-Game Menu");
    m_menu->update(deltaTime);
    if (m_menu->getType() == iMenu::MenuGroup::Skill ||
        m_menu->getType() == iMenu::MenuGroup::GameOver ||
        m_menu->getType() == iMenu::MenuGroup::GameWon ||
        m_menu->getType() == iMenu::MenuGroup::Pause) // Quick "temp pause" fix for testing purposes
        return;
    PROFILE_END();

    ComboMachine::Get().update(deltaTime);


    if (fullhack)
    {
        static_cast<iMenuCards*>(m_menu->getActiveMenu())->setCardInformation(m_cardManager->getHand());
        fullhack = false;
    }

    // Move this somwhere else, don't ruin this class with spagetti & meatballs
    if (m_menu->getType() != iMenu::MenuGroup::CardSelect)
    if (m_waveTimeManager.update(deltaTime, m_entityManager))
    {
        m_menu->queueMenu(iMenu::MenuGroup::CardSelect);
        m_cardManager->pickThreeCards(m_player->getHP() != 3);
        m_projectileManager->removeEnemyProjCallbacks();

        //TODO temp
        fullhack = true;
    }


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

#ifdef _DEBUG
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::NumPad8))
        m_player->takeDamage(1, 0);
#endif // _DEBUG

    if (m_player->getHP() <= 0)
        gameOver();

//    if ((m_waveTimeManager.getOnLastWave() && (m_entityManager.getNrOfAliveEnemies() == 0)) || DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::P))
//        gameWon();
}

void StatePlaying::render() const
{
    // Debug Draw physics
#ifdef _DEBUG
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift))
        m_physics->render();
#endif // _DEBUG

    PROFILE_BEGIN("Player Render");
    if (m_menu->getType() != iMenu::MenuGroup::GameOver)
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
    if (m_menu->getType() != iMenu::MenuGroup::Skill && 
        m_menu->getType() != iMenu::MenuGroup::GameOver &&
        m_menu->getType() != iMenu::MenuGroup::GameWon &&
        m_menu->getType() != iMenu::MenuGroup::Pause)
        m_hudManager.render();
    PROFILE_END();

    PROFILE_BEGIN("Render Menu");
    m_menu->render();
    PROFILE_END();

    m_fpsRenderer.render();
}

void StatePlaying::gameOver()
{
    ComboMachine::Get().endCombo();
    m_highScoreManager->addNewHighScore(ComboMachine::Get().getTotalScore());
    m_menu->queueMenu(iMenu::MenuGroup::GameOver);
    m_menu->startDeathAnimation(m_player->getPosition(), m_player->getForward());
}

void StatePlaying::gameWon()
{
    ComboMachine::Get().endCombo();
    m_highScoreManager->addNewHighScore(ComboMachine::Get().getTotalScore());
    m_menu->queueMenu(iMenu::MenuGroup::GameWon);
    m_menu->startDeathAnimation(m_player->getPosition(), m_player->getForward());
}