#include <Misc\GUI\Specific\iMenuCinematic.h>
#include <Keyboard.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\iMenuMachine.h>
#include <Graphics\include\MainCamera.h>
#include <Engine\Settings.h>
#include <Engine/Engine.h>

using namespace Logic;

#define isKeyDown(key) DirectX::Keyboard::Get().GetState().IsKeyDown(key)

iMenuCinematic::iMenuCinematic(iMenu::MenuGroup group)
    : iMenu(group)
{
    position    = DirectX::SimpleMath::Vector3(0, 5, 0);
    forward     = DirectX::SimpleMath::Vector3(0, 0, 1);

    // Unlock this menu
    mode            = Cinematic_Editing;
    m_isFading      = false;
    m_safeToRemove  = true;
    switchedMode    = false;
    current         = nullptr;
    hideInfo        = false;
    hideImGui       = true;

    edit_text.push_back(L"In Edit Mode");
    edit_text.push_back(L"Tab - Switch Mode");
    edit_text.push_back(L"C - Capture Placement");
    edit_text.push_back(L"V - Print all placements");
    edit_text.push_back(L"B - Remove last placement");
    edit_text.push_back(L"");

    viewing_text.push_back(L"In Viewing Mode");
    viewing_text.push_back(L"Tab - Switch Mode");
    viewing_text.push_back(L"R - Go back to beginning");
    viewing_text.push_back(L"Arrow keys - Switch between placement");
    viewing_text.push_back(L"H - Hide Information");
    viewing_text.push_back(L"I - Toggle Options");
    viewing_text.push_back(L"");

    m_infoText.resize(6);
    for (int i = 0; i < 6; i++)
    {
        m_infoText[i].color = DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 1.f);
        m_infoText[i].font = Resources::Fonts::KG14;
        m_infoText[i].isMoveable = true;
        m_infoText[i].position = DirectX::SimpleMath::Vector2(0.5, (i * 20.f) + 0.5f);
        m_infoText[i].text = edit_text[i].c_str();
    }
}

iMenuCinematic::~iMenuCinematic() { }

void iMenuCinematic::update(int x, int y, float deltaTime)
{
    switch (mode)
    {
    case Cinematic_Editing: updateEdit(); break;
    case Cinematic_Viewing: updatePlay(); break;
    }

    Global::mainCamera->update(position, forward, Global::context);
}

void iMenuCinematic::removeLast()
{
    if (!m_placements.empty())
    {
        printf("Remove the last Placement.\n");
        m_placements.pop_back();
    }
    else 
        printf("All placements are removed.\n");
}

void iMenuCinematic::capture() 
{
    Placement placement;
    placement.position = position;
    placement.forward = forward;
    placement.index = m_placements.size();
    printf("Captured Placement: Index: %d, Pos: %f, %f, %f - Forward: %f, %f, %f\n", placement.index, placement.position.x, placement.position.y, placement.position.z, placement.forward.x, placement.forward.y, placement.forward.z);

    m_placements.push_back(placement);
}

void iMenuCinematic::printPositions() 
{
    system("cls");
    printf("**************************************\n");
    printf("*             Placements             *\n");
    printf("**************************************\n");
    if (m_placements.empty()) printf("No placements, please capture a few, asshat.\n");
    for (size_t i = 0; i < m_placements.size(); i++)
    {
        Placement p = m_placements[i];
        printf("#%d: Index: %d, Pos: %f, %f, %f - Forward: %f, %f, %f\n", i, p.index, p.position.x, p.position.y, p.position.z, p.forward.x, p.forward.y, p.forward.z);
    }
    printf("**************************************\n");
}

void iMenuCinematic::updateEdit() 
{
    DirectX::Mouse::Get().SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
    mouseMovement();

    if (isKeyDown(DirectX::Keyboard::Tab) && !switchedMode)
    {
        for (int i = 0; i < 6; i++)
            m_infoText[i].text = viewing_text[i].c_str();

        mode = Cinematic_Viewing;
        switchedMode = true;
        current = nullptr;
        currentIndex = 0;
    }
    else if (!isKeyDown(DirectX::Keyboard::Tab) && switchedMode)
    {
        switchedMode = false;
    }

    static bool buttonPressed = false;

    if (isKeyDown(DirectX::Keyboard::C) && !buttonPressed)
    {
        capture();
        buttonPressed = true;
    }

    if (isKeyDown(DirectX::Keyboard::V) && !buttonPressed)
    {
        printPositions();
        buttonPressed = true;
    }

    if (isKeyDown(DirectX::Keyboard::B) && !buttonPressed)
    {
        removeLast();
        buttonPressed = true;
    }

    if (!isKeyDown(DirectX::Keyboard::C) &&
        !isKeyDown(DirectX::Keyboard::V) &&
        !isKeyDown(DirectX::Keyboard::B))
        buttonPressed = false;

    static float speed = 0.1f;
    if (isKeyDown(DirectX::Keyboard::A))
        position -= speed * DirectX::SimpleMath::Vector3(forward.x, 0, forward.z).Cross({ 0, 1, 0 });
    if (isKeyDown(DirectX::Keyboard::D))
        position += speed * DirectX::SimpleMath::Vector3(forward.x, 0, forward.z).Cross({ 0, 1, 0 });
    if (isKeyDown(DirectX::Keyboard::W))
        position += speed * DirectX::SimpleMath::Vector3(forward.x, 0, forward.z);
    if (isKeyDown(DirectX::Keyboard::S))
        position -= speed * DirectX::SimpleMath::Vector3(forward.x, 0, forward.z);
    if (isKeyDown(DirectX::Keyboard::LeftShift))
        position.y += 0.15f;
    if (isKeyDown(DirectX::Keyboard::LeftControl))
        position.y -= 0.15f;
}

void iMenuCinematic::updatePlay() 
{
    DirectX::Mouse::Get().SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);

    if (isKeyDown(DirectX::Keyboard::Tab) && !switchedMode)
    {
        for (int i = 0; i < 6; i++)
            m_infoText[i].text = edit_text[i].c_str();

        mode = Cinematic_Editing;
        switchedMode = true;
        hideInfo = false;
    }
    else if (!isKeyDown(DirectX::Keyboard::Tab) && switchedMode)
    {
        switchedMode = false;
    }


    static bool buttonPressed = false;
    if (isKeyDown(DirectX::Keyboard::Left) && !buttonPressed)
    {
        current = nullptr;
        currentIndex--;
        buttonPressed = true;
    }
    if (isKeyDown(DirectX::Keyboard::Right) && !buttonPressed)
    {
        current = nullptr;
        currentIndex++;
        buttonPressed = true;
    }
    if (isKeyDown(DirectX::Keyboard::R) && !buttonPressed)
    {
        current = nullptr;
        currentIndex = 0;
        buttonPressed = true;
    }
    if (isKeyDown(DirectX::Keyboard::H) && !buttonPressed)
    {
        hideInfo = !hideInfo;
        buttonPressed = true;
    }
    if (isKeyDown(DirectX::Keyboard::I) && !buttonPressed)
    {
        hideImGui = !hideImGui;
        buttonPressed = true;
    }

    if (!isKeyDown(DirectX::Keyboard::Right) &&
        !isKeyDown(DirectX::Keyboard::Left) &&
        !isKeyDown(DirectX::Keyboard::R) &&
        !isKeyDown(DirectX::Keyboard::H) && 
        !isKeyDown(DirectX::Keyboard::I))
        buttonPressed = false;

    play();
}

void iMenuCinematic::play()
{
    if (m_placements.empty())
    {
        printf("Can't play without any placements\n");
        return;
    }

    // If the cinematic have no current placement
    if (!current) 
    {
        if (m_placements.size() - 1 >= currentIndex)
            current = &m_placements[currentIndex]; 
        else 
        {
            printf("Restarting\n");
            currentIndex = 0;
        }
    }

    easeTo();
}

void iMenuCinematic::easeTo()
{
    static float posSpeed = 0.0040f, forSpeed = 0.0010f;
    static float distancePos = 18.f, distanceFor = 0.50f;
    if (!hideImGui)
    {
        ImGui::Begin("Speeds");
        ImGui::DragFloat("Position Speed: ", &posSpeed, 0.00001f, 0.0001f, 0.0100f, "%.6f");
        ImGui::DragFloat("Forward Speed: ", &forSpeed, 0.00001f, 0.0001f, 0.0100f, "%.6f");
        ImGui::DragFloat("Position Difference: ", &distancePos, 0.001f, 0.0f, 50.f, "%.6f");
        ImGui::DragFloat("Forward Difference: ", &distanceFor, 0.0001f, 0.1f, 1.f, "%.6f");
        ImGui::End();
    }

    if (current)
    {
        position    += (current->position - position) * posSpeed;
        forward     += (current->forward - forward)   * forSpeed;

        DirectX::SimpleMath::Vector3 diff_1 = position - current->position;
        DirectX::SimpleMath::Vector3 diff_2 = forward - current->forward;

        float distance_1 = sqrtf(diff_1.Dot(diff_1));
        float distance_2 = sqrtf(diff_2.Dot(diff_2));

        if (distance_1 < 18.f && distance_2 < 0.50f)
        {
            current = nullptr;
            currentIndex++;
        }
    }
}

void iMenuCinematic::render() const
{
    if (!hideInfo)
    {
        iMenu::render();

        for (int i = 0; i < 6; i++)
            QueueRender(m_infoText[i]);
    }
}

void iMenuCinematic::mouseMovement()
{
    // so bad lol
    POINT mousePos;
    GetCursorPos(&mousePos);
    auto vec2 = getWindowMidPoint();

    float xDiff = mousePos.x - vec2.x;
    int yDiff = mousePos.y - vec2.y;

    m_camYaw    += Settings::getInstance().getMouseSense() * 1.5f * xDiff;
    m_camPitch  -= Settings::getInstance().getMouseSense() * yDiff;
    SetCursorPos(vec2.x, vec2.y);

    // Pitch lock and yaw correction
    if (m_camPitch > 89)
        m_camPitch = 89;
    if (m_camPitch < -89)
        m_camPitch = -89;
    if (m_camYaw < 0.f)
        m_camYaw += 360.f;
    if (m_camYaw > 360.f)
        m_camYaw -= 360.f;

    // Create forward
    forward.x = cos(DirectX::XMConvertToRadians(m_camPitch)) * cos(DirectX::XMConvertToRadians(m_camYaw));
    forward.y = sin(DirectX::XMConvertToRadians(m_camPitch));
    forward.z = cos(DirectX::XMConvertToRadians(m_camPitch)) * sin(DirectX::XMConvertToRadians(m_camYaw));

    forward.Normalize();
}

DirectX::SimpleMath::Vector2 iMenuCinematic::getWindowMidPoint()
{
    RECT rect;
    GetWindowRect(*Engine::g_window, &rect);
    return DirectX::SimpleMath::Vector2((rect.left + rect.right) * 0.5f, (rect.top + rect.bottom) * 0.5f);
}