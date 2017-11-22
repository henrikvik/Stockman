#include <Misc\GUI\iMenuMachine.h>
#include <Misc\GUI\iMenuFactory.h>
#include <Misc\GUI\iMenuAction.h>
#include <Graphics\include\MainCamera.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <Misc\Sound\NoiseMachine.h>

#define EDIT_CAMERA_POS false       // Editing Mode - For getting camera placements
#if EDIT_CAMERA_POS 
#include <imgui.h>
#endif

#define CAMERA_MOVE_SPEED           0.00055f // The speed of the camera movement
#define CAMERA_START_POSITION       DirectX::SimpleMath::Vector3(10.00, 7.804, -8.984)
#define CAMERA_START_FORWARD        DirectX::SimpleMath::Vector3(0.027, -0.192, 0.789)
#define CAMERA_SETTINGS_POSITION    DirectX::SimpleMath::Vector3(5.294, 1.843, -10.0)
#define CAMERA_SETTINGS_FORWARD     DirectX::SimpleMath::Vector3(0.0, 1.0, 0.0)
#define CAMERA_HIGHSCORE_POSITION   DirectX::SimpleMath::Vector3(-2.549, 1.686, -3.125)
#define CAMERA_HIGHSCORE_FORWARD    DirectX::SimpleMath::Vector3(0.341, 0.012, 0.352)
#define CAMERA_INTRO_POSITION       DirectX::SimpleMath::Vector3(8.118, 1.059, -3.047)
#define CAMERA_INTRO_FORWARD        DirectX::SimpleMath::Vector3(-0.027, 1.000, 0.656)
#define CAMERA_SKILL_POSITION       DirectX::SimpleMath::Vector3(-10.000, 35.00, 0.000)
#define CAMERA_SKILL_FORWARD        DirectX::SimpleMath::Vector3(0.000, -0.365, 0.023)

using namespace Logic;

iMenuMachine::iMenuMachine()
{
    m_deathPosition = DirectX::SimpleMath::Vector3(0, 0, 0);
    m_deathForward = DirectX::SimpleMath::Vector3(0, 0, 0);
    m_resetDeathPositionAndForward = true;

    m_factory = newd iMenuFactory;
    m_activeMenu = nullptr;

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

void iMenuMachine::queueMenu(iMenu::MenuGroup group)
{
    if (m_currentMenuType != group)
    {
        m_queuedMenuType = group;
        
        if (m_activeMenu)
        {
            m_activeMenu->fadeOut();
        }
    }
}

void iMenuMachine::swapMenu()
{
    removeActiveMenu();

    switch (m_queuedMenuType)
    {
    case iMenu::MenuGroup::Intro:               m_activeMenu = m_factory->buildMenuIntro();                       break;
    case iMenu::MenuGroup::Start:               m_activeMenu = m_factory->buildMenuStart();                       break;
    case iMenu::MenuGroup::Settings:            m_activeMenu = m_factory->buildMenuSettings();                    break;
    case iMenu::MenuGroup::Highscore:           m_activeMenu = m_factory->buildMenuHighscore();                   break;
    case iMenu::MenuGroup::CardSelect:          m_activeMenu = m_factory->buildMenuCard();                        break;
    case iMenu::MenuGroup::Skill:               m_activeMenu = m_factory->buildMenuSkill();                       break;
    case iMenu::MenuGroup::GameOver:            m_activeMenu = m_factory->buildMenuGameover();                    break;
    case iMenu::MenuGroup::GameWon:             m_activeMenu = m_factory->buildMenuGameWon();                     break;
    case iMenu::MenuGroup::Pause:               m_activeMenu = m_factory->buildMenuPause();                       break;
    case iMenu::MenuGroup::Loading:             m_activeMenu = m_factory->buildMenuLoading();                     break;
    case iMenu::MenuGroup::HighscoreGameOver:   m_activeMenu = m_factory->buildMenuHighscoreGameOver();           break;
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
    if (m_activeMenu)
    {
        if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Keyboard::Escape) && m_currentMenuType == iMenu::Pause && !m_activeMenu->getIsFading())
        {
            Sound::NoiseMachine::Get().playSFX(Sound::SFX::UI_BUTTON_PRESS, nullptr, true);
            queueMenu(iMenu::MenuGroup::Empty);
        }
    }
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Keyboard::Escape) && m_currentMenuType == iMenu::Empty)
    {
        Sound::NoiseMachine::Get().playSFX(Sound::SFX::UI_BUTTON_PRESS, nullptr, true);
        queueMenu(iMenu::MenuGroup::Pause);
    }

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
        int x = DirectX::Mouse::Get().GetState().x;
        int y = DirectX::Mouse::Get().GetState().y;
        m_activeMenu->update(x, y, deltaTime);
        updateCamera(deltaTime);
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
    ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&targetCameraPosition), -10.f, 10.f, "%.3f");
    ImGui::SliderFloat3("Forward", reinterpret_cast<float*>(&targetCameraForward), -1.f, 1.f, "%.3f");
    shouldModifyCamera = true;
    ImGui::End();
#else

    switch (m_activeMenu->getMenuType())
    {

    case iMenu::MenuGroup::Intro:
        targetCameraPosition = CAMERA_INTRO_POSITION;
        targetCameraForward = CAMERA_INTRO_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::Start:
        targetCameraPosition = CAMERA_START_POSITION;
        targetCameraForward = CAMERA_START_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::Settings:
        targetCameraPosition = CAMERA_SETTINGS_POSITION;
        targetCameraForward = CAMERA_SETTINGS_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::Highscore:
        targetCameraPosition = CAMERA_HIGHSCORE_POSITION;
        targetCameraForward = CAMERA_HIGHSCORE_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::Skill:
        targetCameraPosition = CAMERA_SKILL_POSITION;
        targetCameraForward = CAMERA_SKILL_FORWARD;
        shouldModifyCamera = true;
        break;

    case iMenu::MenuGroup::Loading:
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
