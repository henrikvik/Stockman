#include <StatePlaying.h>
#include <StateMachine\StateBuffer.h>
#include <State.h>
#include <Misc\GUI\Specific\iMenuCards.h>
#include <Misc\Network\dbConnect.h>
#include <Misc\CommandsFile.h>

// Input Singletons
#include <Keyboard.h>
#include <Mouse.h>

// Debugging purposes
#include <DebugDefines.h>
#include <Engine\Settings.h>
#include <Singletons\DebugWindow.h> 
#include <GameType.h>

#include <toml\toml.h>

using namespace Logic;

// Game starting static configurations
const int StatePlaying::GAME_START::UNIQUE_CARDS = 13;
const btVector3 StatePlaying::GAME_START::PLAYER_SCALE = { 1.5f, 3.0f, 1.5f };
const btVector3 StatePlaying::GAME_START::PLAYER_ROTATION = { 0.0f, 0.0f, 0.0f };

static void FillLightVec(std::vector<LightRenderInfo> &lights, std::string path)
{
    auto val = toml::parseFile(path).value;

    for (auto light : val.find("lights")->as<toml::Array>()) {
        auto position = light["position"].as<toml::Array>();
        auto col = light["color"].as<toml::Array>();
        auto radius = light["range"].asNumber();
        auto intensity = light["intensity"].asNumber();

        LightRenderInfo info = {};
        info.position.x = position[0].asNumber();
        info.position.y = position[1].asNumber();
        info.position.z = position[2].asNumber();
        info.color.x = col[0].asNumber();
        info.color.y = col[1].asNumber();
        info.color.z = col[2].asNumber();
        info.range = radius;
        info.intensity = intensity;

        lights.push_back(info);
    }
}

static void SubmitLights(std::vector<LightRenderInfo> &lights)
{
    for (auto light : lights) {
        QueueRender(light);
    }
}

StatePlaying::StatePlaying(StateBuffer* stateBuffer)
    : State(stateBuffer),
      m_ChristmasLightTimer(1.5f),
      m_ChristmasPatternIndex(0)
{
    // Starting in game-sounds
    Sound::NoiseMachine::Get().stopGroup(Sound::CHANNEL_SFX);
    Sound::NoiseMachine::Get().loadPlaySounds();
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::AMBIENT_STORM, nullptr, true);
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::MUSIC_IN_GAME, nullptr, true);

    // Initializing Bullet physics
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();		// Configuration
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);	                // The default collision dispatcher
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();								    // Detecting aabb-overlapping object pairs
    btSequentialImpulseConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver();		// Default constraint solver
    m_physics = new Physics(dispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);
    m_physics->init();

    // Initializing Projectile Manager
    m_projectileManager = newd ProjectileManager(m_physics);

    // Initializing Player
    m_player = newd Player(Resources::Models::UnitCube, nullptr, GAME_START::PLAYER_SCALE);
    m_player->init(m_physics, m_projectileManager);
    Sound::NoiseMachine::Get().update(m_player->getListenerData());

    // static map lights
    FillLightVec(m_MapLights, "../Resources/Maps/lights.toml");
    FillLightVec(m_RedBulbs, "../Resources/Maps/r.toml");
    FillLightVec(m_GreenBulbs, "../Resources/Maps/g.toml");
    FillLightVec(m_BlueBulbs, "../Resources/Maps/b.toml");

    //                                       bgr
    m_ChristmasLightPattern.push_back(0b00000000);
    m_ChristmasLightPattern.push_back(0b00000001);
    m_ChristmasLightPattern.push_back(0b00000010);
    m_ChristmasLightPattern.push_back(0b00000100);
    m_ChristmasLightPattern.push_back(0b00000001);
    m_ChristmasLightPattern.push_back(0b00000011);
    m_ChristmasLightPattern.push_back(0b00000111);
    m_ChristmasLightPattern.push_back(0b00000110);
    m_ChristmasLightPattern.push_back(0b00000100);

    // Initializing the Map
    m_map = newd Map();
    m_map->init(m_physics);
    m_map->loadMap(Resources::Maps::Stock_Map);

    // Initializing Card Manager
    m_cardManager = newd CardManager(GAME_START::UNIQUE_CARDS);

    // Initializing Combo's
    ComboMachine::Get().reset();

    // Initializing Menu's
    m_menu = newd iMenuMachine();
    m_menu->queueMenu(iMenu::MenuGroup::LoadingPost);

    // Loading func
    m_entityManager.setSpawnFunctions(*m_projectileManager, *m_physics);

    CommandsFile().doCommandsFromFile();
    RenderQueue::get().clearAllQueues();

    //temp? probably NOT
    static SpecialEffectRenderInfo info = {};
    info.type = info.Snow;
    info.restart = true;
    
    m_playTime = 0;
    

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
    delete m_projectileManager;

    if (highscoreThread.joinable())
    {
        highscoreThread.join();
    }
    Sound::NoiseMachine::Get().clearCurrent();
}

void StatePlaying::reset()
{
    m_playTime = 0;

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
    m_ChristmasLightTimer += deltaTime / 1000.f;
    if (m_ChristmasLightTimer > 1.5f) {
        m_ChristmasLightTimer = 0.f;
        m_ChristmasPatternIndex = (m_ChristmasPatternIndex + 1) % m_ChristmasLightPattern.size();
    }

    auto pattern = m_ChristmasLightPattern[m_ChristmasPatternIndex];

    if (pattern & 0x1)
        SubmitLights(m_RedBulbs);
    if (pattern & 0x2)
        SubmitLights(m_GreenBulbs);
    if (pattern & 0x4)
        SubmitLights(m_BlueBulbs);

    SubmitLights(m_MapLights);

    m_fpsRenderer.updateFPS(deltaTime);
    PROFILE_BEGIN("cards");
    m_cardManager->update(deltaTime);
    PROFILE_END();


    PROFILE_BEGIN("HUD");
    m_hudManager.update(*m_player, m_waveTimeManager, m_entityManager, deltaTime);
    PROFILE_END();
   
    PROFILE_BEGIN("In-Game Menu");
    m_menu->update(deltaTime);
    if (m_menu->getType() != iMenu::Empty && m_menu->getType() != iMenu::Cinematic) // Quick "temp pause" fix for testing purposes
        return;
    PROFILE_END();

        m_playTime += deltaTime;
        ComboMachine::Get().update(deltaTime);

        // Move this somwhere else, don't ruin this class with spagetti & meatballs
        //the spagetti is (expand)ing (dong)
        if (m_menu->getType() != iMenu::CardSelect)
        {
            float tempTime = m_waveTimeManager.getTimeCurrent();
            bool newWave = m_waveTimeManager.update(deltaTime, m_entityManager, m_player->getPositionBT());

            if (newWave)
            {
                ComboMachine::Get().addTimeBonus(tempTime);
                m_menu->queueMenu(iMenu::CardSelect);
                m_cardManager->pickThreeCards(m_player->getHP() != m_player->getMaxHP());
                m_projectileManager->removeAllProjectiles();
            }
        }

        PROFILE_BEGIN("Sound");
        Sound::NoiseMachine::Get().update(m_player->getListenerData());
        PROFILE_END();

        PROFILE_BEGIN("Player");
        if (m_menu->getType() != iMenu::Cinematic)
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

#define _DEBUG
#ifdef  _DEBUG
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::NumPad8))
        m_player->takeDamage(1, 0);
#endif // _DEBUG

    if (m_player->getHP() <= 0)
        gameOver();

    if ((m_waveTimeManager.getOnLastWave() && (m_entityManager.getNrOfAliveEnemies() == 0)) || DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::P))
        gameWon();
}

void StatePlaying::render() const
{
    // Debug Draw physics
#ifdef _DEBUG
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftShift))
        m_physics->render();
#endif // _DEBUG

    PROFILE_BEGIN("Player Render");
    if (m_menu->getType() != iMenu::GameOver)
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
    if (m_menu->getType() == iMenu::Empty ||
        m_menu->getType() == iMenu::CardSelect)
        m_hudManager.render();
    PROFILE_END();

    PROFILE_BEGIN("Render Menu");
    m_menu->render();
    PROFILE_END();

    PROFILE_BEGIN("Render cards");
    if (m_menu->getType() == iMenu::CardSelect)
        m_cardManager->render();
    PROFILE_END();

    m_fpsRenderer.render();
}

void StatePlaying::gameOver()
{
    // Upload score
    addHighscore();

    // Queue Death Screen
    Sound::NoiseMachine::Get().playSFX(Sound::SFX::WAVE_DEAD, nullptr, true);
    m_menu->queueMenu(iMenu::MenuGroup::GameOver);
    m_menu->startDeathAnimation(m_player->getPosition(), m_player->getForward());
    m_hudManager.reset();
}

void StatePlaying::gameWon()
{
    // Upload score
    addHighscore();

    reset();
    m_menu->queueMenu(iMenu::MenuGroup::GameWon);
}

void StatePlaying::addHighscore()
{
    ComboMachine::Get().endCombo();
    Network::dbConnect db;
    std::string name = Settings::getInstance().getName();
    if (name.empty()) name = "Stockman";

    if (highscoreThread.joinable())
        highscoreThread.join();

    highscoreThread = std::thread(Network::dbConnect::addHighscore, name, ComboMachine::Get().getTotalScore(), int(m_playTime * 0.001f), m_waveTimeManager.getCurrentWave(), ComboMachine::Get().getTotalKills());
}
