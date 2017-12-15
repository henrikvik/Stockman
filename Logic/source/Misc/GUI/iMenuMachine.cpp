#include <Misc\GUI\iMenuMachine.h>
#include <Misc\GUI\iMenuFactory.h>
#include <Misc\GUI\iMenuAction.h>
#include <Graphics\include\MainCamera.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Singletons\Profiler.h>

#define PAUSE_BUTTON                DirectX::Keyboard::Keyboard::Escape
#define CONTROLS_BUTTON             DirectX::Keyboard::Keyboard::F1
#define CINEMATIC_BUTTON            DirectX::Keyboard::Keyboard::F9

#define EDIT_CAMERA_POS false       // Editing Mode - For getting camera placements
#if EDIT_CAMERA_POS 
#include <imgui.h>
#endif

#define CAMERA_MOVE_SPEED           0.00055f // The speed of the camera movement
#define CAMERA_START_POSITION       DirectX::SimpleMath::Vector3(-18.627, 32.353, -140.265)
#define CAMERA_START_FORWARD        DirectX::SimpleMath::Vector3(0.506, -0.231, 0.99)
#define CAMERA_SETTINGS_POSITION    DirectX::SimpleMath::Vector3(-77.451, 45.00, -54.600)
#define CAMERA_SETTINGS_FORWARD     DirectX::SimpleMath::Vector3(0.624, -0.286, 0.203)
#define CAMERA_HIGHSCORE_POSITION   DirectX::SimpleMath::Vector3(20.588, 40.197, -99.609)
#define CAMERA_HIGHSCORE_FORWARD    DirectX::SimpleMath::Vector3(0.608, -0.294, 0.383)
#define CAMERA_INTRO_POSITION       DirectX::SimpleMath::Vector3(-18.627, 36.275, -140.265)
#define CAMERA_INTRO_FORWARD        DirectX::SimpleMath::Vector3(0.506, 0.99f, 0.99f)
#define CAMERA_SKILL_POSITION       DirectX::SimpleMath::Vector3(-10.000, 35.00, 0.000)
#define CAMERA_SKILL_FORWARD        DirectX::SimpleMath::Vector3(0.000, -0.365, 0.023)
#define CAMERA_CREDITS_POSITION     DirectX::SimpleMath::Vector3(-73.529, 26.471, -64.453)
#define CAMERA_CREDITS_FORWARD      DirectX::SimpleMath::Vector3(0.741, -0.271, 0.523)
#define CAMERA_GAMEWON_POSITION       DirectX::SimpleMath::Vector3(-10.000, 70.00, 0.000)
#define CAMERA_GAMEWON_FORWARD        DirectX::SimpleMath::Vector3(0.000, -0.365, 0.023)
/* 

    Campfire Map Camera Positions

    DirectX::SimpleMath::Vector3(10.00, 7.804, -8.984)
    DirectX::SimpleMath::Vector3(0.027, -0.192, 0.789)
    DirectX::SimpleMath::Vector3(5.294, 1.843, -10.0)
    DirectX::SimpleMath::Vector3(0.0, 1.0, 0.0)
    DirectX::SimpleMath::Vector3(-2.549, 1.686, -3.125)
    DirectX::SimpleMath::Vector3(0.341, 0.012, 0.352)
    DirectX::SimpleMath::Vector3(8.118, 1.059, -3.047)
    DirectX::SimpleMath::Vector3(-0.027, 1.000, 0.656)
    DirectX::SimpleMath::Vector3(-10.000, 35.00, 0.000)
    DirectX::SimpleMath::Vector3(0.000, -0.365, 0.023)

*/

using namespace Logic;

iMenuMachine::iMenuMachine()
{
    m_deathPosition = DirectX::SimpleMath::Vector3(0, 0, 0);
    m_deathForward = DirectX::SimpleMath::Vector3(0, 0, 0);
    m_resetDeathPositionAndForward = true;

    m_factory = newd iMenuFactory;
    m_activeMenu = nullptr;

    m_queuedMenuType = iMenu::MenuGroup::Empty;
    m_currentMenuType = iMenu::MenuGroup::Empty;

    Action::Get().SetPointer(this);
}

iMenuMachine::~iMenuMachine()
{
    removeActiveMenu();
    delete m_factory;
}

void iMenuMachine::removeActiveMenu()
{
    if (m_activeMenu)
    {
        delete m_activeMenu;
        m_activeMenu = nullptr;
    }
}

void iMenuMachine::queueMenuWithSound(iMenu::MenuGroup group)
{
    Sound::NoiseMachine::Get().playSFX(Sound::SFX::UI_BUTTON_PRESS, nullptr, true);
    queueMenu(group);
}

void iMenuMachine::queueMenu(iMenu::MenuGroup group)
{
    if (m_currentMenuType != group)
    {
        if (m_queuedMenuType != group)
        {
            m_queuedMenuType = group;

            if (m_activeMenu)
            {
                m_activeMenu->fadeOut();
            }
        }
    }
}

void iMenuMachine::swapMenu()
{
    removeActiveMenu();

    switch (m_queuedMenuType)
    {
    case iMenu::MenuGroup::Intro:               m_activeMenu = m_factory->buildMenuIntro();             break;
    case iMenu::MenuGroup::FirstTime:           m_activeMenu = m_factory->buildMenuFirstTime();         break;
    case iMenu::MenuGroup::Start:               m_activeMenu = m_factory->buildMenuStart();             break;
    case iMenu::MenuGroup::SettingsStart:       m_activeMenu = m_factory->buildMenuSettings();          break;
    case iMenu::MenuGroup::SettingsPause:       m_activeMenu = m_factory->buildMenuPauseSettings();     break;
    case iMenu::MenuGroup::HighscoreStartMenu:  m_activeMenu = m_factory->buildMenuHighscore();         break;
    case iMenu::MenuGroup::CardSelect:          m_activeMenu = m_factory->buildMenuCard();              break;
    case iMenu::MenuGroup::Skill:               m_activeMenu = m_factory->buildMenuSkill();             break;
    case iMenu::MenuGroup::GameOver:            m_activeMenu = m_factory->buildMenuGameover();          break;
    case iMenu::MenuGroup::GameWon:             m_activeMenu = m_factory->buildMenuGameWon();           break;
    case iMenu::MenuGroup::Pause:               m_activeMenu = m_factory->buildMenuPause();             break;
    case iMenu::MenuGroup::Controls:            m_activeMenu = m_factory->buildMenuControls();          break;
    case iMenu::MenuGroup::LoadingPre:          m_activeMenu = m_factory->buildMenuLoadingPre();        break;
    case iMenu::MenuGroup::LoadingPost:         m_activeMenu = m_factory->buildMenuLoadingPost();       break;
    case iMenu::MenuGroup::HighscoreGameOver:   m_activeMenu = m_factory->buildMenuHighscoreGameOver(); break;
    case iMenu::MenuGroup::Cinematic:           m_activeMenu = m_factory->buildMenuCinematic();         break;
    case iMenu::MenuGroup::Credits:             m_activeMenu = m_factory->buildMenuCredits();           break;
    default: break;
    }

    // New menu! Fade it in.
    if (m_activeMenu) m_activeMenu->fadeIn();

    // No active menu, unlock mouse.
    else DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);

    m_currentMenuType = m_queuedMenuType;
}

void iMenuMachine::update(float deltaTime)
{
    // Enter and exit Trailer Mode
    if (m_activeMenu)
    {
        if (m_activeMenu->getMenuType() != iMenu::Cinematic && DirectX::Keyboard::Get().GetState().IsKeyDown(CINEMATIC_BUTTON) && !m_activeMenu->getIsFading())
        {
            queueMenuWithSound(iMenu::MenuGroup::Cinematic);
        }
        if (m_activeMenu->getMenuType() == iMenu::Cinematic && DirectX::Keyboard::Get().GetState().IsKeyDown(CINEMATIC_BUTTON) && !m_activeMenu->getIsFading())
        {
            queueMenuWithSound(iMenu::MenuGroup::Start);
        }
    }

    // The in-game pause menu
    if (m_activeMenu)
        if (DirectX::Keyboard::Get().GetState().IsKeyDown(PAUSE_BUTTON) && m_currentMenuType == iMenu::Pause && !m_activeMenu->getIsFading())
            queueMenuWithSound(iMenu::MenuGroup::Empty);
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(PAUSE_BUTTON) && m_currentMenuType == iMenu::Empty)
        queueMenuWithSound(iMenu::MenuGroup::Pause);

    // The controls menu
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(CONTROLS_BUTTON) && m_currentMenuType == iMenu::Empty)
        queueMenuWithSound(iMenu::MenuGroup::Controls);

    if (wantsToSwap())
    {
        if (m_activeMenu)
        {
            if (m_activeMenu->getIsSafeToRemove())
            {
                swapMenu();
                return;
            }
        }
        else
        {
            swapMenu();
            return;
        }
    }

    if (m_activeMenu)
    {
        PROFILE_BEGIN("Menu Update");
        int x = DirectX::Mouse::Get().GetState().x;
        int y = DirectX::Mouse::Get().GetState().y;
        m_activeMenu->update(x, y, deltaTime);
        PROFILE_END();

        PROFILE_BEGIN("Camera Swoosh");
        updateCamera(deltaTime);
        PROFILE_END();
    }
}

void iMenuMachine::updateCamera(float deltaTime)
{
    static DirectX::SimpleMath::Vector3 movingCameraPosition(0, 2, 0);
    static DirectX::SimpleMath::Vector3 movingCameraForward(0, 1, 0);
    static DirectX::SimpleMath::Vector3 targetCameraPosition(0, 2, 0);
    static DirectX::SimpleMath::Vector3 targetCameraForward(0, 0, 1);
    bool shouldModifyCamera = false;

    // Debugging purposes
#if EDIT_CAMERA_POS
    ImGui::Begin("Camera");
    ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&targetCameraPosition), -250.f, 250.f, "%.3f");
    ImGui::SliderFloat3("Forward", reinterpret_cast<float*>(&targetCameraForward), -1.f, 1.f, "%.3f");
    shouldModifyCamera = true;
    ImGui::End();
#else

    switch (m_activeMenu->getMenuType())
    {
    case iMenu::MenuGroup::FirstTime:
    case iMenu::MenuGroup::Intro:
    case iMenu::MenuGroup::Credits:
        targetCameraPosition = CAMERA_INTRO_POSITION;
        targetCameraForward = CAMERA_INTRO_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::Start:
        targetCameraPosition = CAMERA_START_POSITION;
        targetCameraForward = CAMERA_START_FORWARD;
        shouldModifyCamera = true;
        break;
    
    case iMenu::MenuGroup::SettingsStart:
        targetCameraPosition = CAMERA_SETTINGS_POSITION;
        targetCameraForward = CAMERA_SETTINGS_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::HighscoreStartMenu:
        targetCameraPosition = CAMERA_HIGHSCORE_POSITION;
        targetCameraForward = CAMERA_HIGHSCORE_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::LoadingPre:
    case iMenu::MenuGroup::LoadingPost:
    case iMenu::MenuGroup::Skill:
        targetCameraPosition = CAMERA_SKILL_POSITION;
        targetCameraForward = CAMERA_SKILL_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::GameOver:
        targetCameraPosition = m_deathPosition + DirectX::SimpleMath::Vector3(0, 20, 0);
        targetCameraForward = DirectX::SimpleMath::Vector3(0.000, -0.365, 0.023);
        shouldModifyCamera = true;
        if (m_resetDeathPositionAndForward)
        {
            movingCameraPosition = m_deathPosition;
            movingCameraForward = m_deathForward;
            m_resetDeathPositionAndForward = false;
        }
        break;

    case iMenu::MenuGroup::GameWon:
        targetCameraPosition = CAMERA_GAMEWON_POSITION;
        targetCameraForward = CAMERA_GAMEWON_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::Cinematic:
        shouldModifyCamera = false;
        break;
    }
#endif

    // Moving the camera slightly if allowed
    if (shouldModifyCamera)
    {
        movingCameraPosition += (targetCameraPosition - movingCameraPosition) * CAMERA_MOVE_SPEED * deltaTime;
        movingCameraForward += (targetCameraForward - movingCameraForward) * CAMERA_MOVE_SPEED * deltaTime;
        Global::mainCamera->update(movingCameraPosition, movingCameraForward, Global::context);
    }
}

void iMenuMachine::render() const
{
    if (m_activeMenu)
        m_activeMenu->render();
}

void iMenuMachine::startDeathAnimation(Vector3 position, Vector3 forward)
{
    m_deathPosition = position;
    m_deathForward = forward;
    m_resetDeathPositionAndForward = true;
}

bool iMenuMachine::wantsToSwap()
{
    return (m_currentMenuType != m_queuedMenuType);
}
