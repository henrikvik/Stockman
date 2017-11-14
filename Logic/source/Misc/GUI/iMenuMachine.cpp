#include <Misc\GUI\iMenuMachine.h>
#include <Misc\GUI\iMenuFactory.h>
#include <Misc\GUI\iMenuAction.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <Graphics\include\MainCamera.h>
#include <imgui.h>

#define EDIT_CAMERA_POS false       // Editing Mode
#define CAMERA_MOVE_SPEED           0.001f // The speed of the camera movement
#define CAMERA_START_POSITION       DirectX::SimpleMath::Vector3(6.941, 5.6, -4.141)
#define CAMERA_START_FORWARD        DirectX::SimpleMath::Vector3(-0.2, -0.153, 0.258)
#define CAMERA_SETTINGS_POSITION    DirectX::SimpleMath::Vector3(5.294, 1.843, -10.0)
#define CAMERA_SETTINGS_FORWARD     DirectX::SimpleMath::Vector3(0.0, -1.0, 0.0)
#define CAMERA_HIGHSCORE_POSITION   DirectX::SimpleMath::Vector3(2.471, 1.686, -2.500)
#define CAMERA_HIGHSCORE_FORWARD    DirectX::SimpleMath::Vector3(1.0, 0.255, 0.828)

using namespace Logic;

iMenuMachine::iMenuMachine()
{
    m_factory = newd iMenuFactory;
    m_activeMenu = nullptr;

    Action::Get().SetPointer(this);
}

iMenuMachine::~iMenuMachine()
{
    removeActiveMenu();
}

void iMenuMachine::removeActiveMenu()
{
    if (m_activeMenu)
    {
        delete m_activeMenu;
        m_activeMenu = nullptr;
    }
}

void iMenuMachine::swapMenu(iMenu::MenuGroup group)
{
    removeActiveMenu();

    switch (group)
    {
    case iMenu::MenuGroup::Start:       m_activeMenu = m_factory->buildMenuStart();                       break;
    case iMenu::MenuGroup::Settings:    m_activeMenu = m_factory->buildMenuSettings();                    break;
    case iMenu::MenuGroup::Highscore:   m_activeMenu = m_factory->buildMenuHighscore();                   break;
    case iMenu::MenuGroup::Card:        m_activeMenu = m_factory->buildMenuCard();                        break;
    case iMenu::MenuGroup::Skill:       m_activeMenu = m_factory->buildMenuSkill();                       break;
    case iMenu::MenuGroup::GameOver:    m_activeMenu = m_factory->buildMenuGameover();                    break;
    default: printf("This type of menu is not created yet. Check: %s, Line: %d", __FILE__, __LINE__);   break;
    }
}

void iMenuMachine::update(float deltaTime)
{
    static DirectX::SimpleMath::Vector3 movingCameraPosition(0, 0, 0);
    static DirectX::SimpleMath::Vector3 movingCameraForward(0, -1, 0);
    static DirectX::SimpleMath::Vector3 targetCameraPosition(0, 1, 0);
    static DirectX::SimpleMath::Vector3 targetCameraForward(0, 0, 1);

    if (m_activeMenu)
    {
        int x = DirectX::Mouse::Get().GetState().x;
        int y = DirectX::Mouse::Get().GetState().y;
        m_activeMenu->update(x, y);

        bool shouldModifyCamera = false;

    // Debugging purposes
    #if EDIT_CAMERA_POS
        ImGui::Begin("Camera");
        ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&targetCameraPosition), -10.f, 10.f, "%.3f");
        ImGui::SliderFloat3("Forward", reinterpret_cast<float*>(&targetCameraForward), -1.f, 1.f, "%.3f");
        menuShouldUseCamera = true;
        ImGui::End();
    #else

        switch (m_activeMenu->getMenuType())
        {
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
}

void iMenuMachine::render() const
{
    if (m_activeMenu)
        m_activeMenu->render();
}
