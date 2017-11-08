#include <Player\PlayerController.h>
// #include <Player\PlayerGeneral.h>
#include <Player\PlayerMovement.h>

using namespace Logic;

PlayerController::PlayerController()
{
    LoadKeyBindings();
}

PlayerController::~PlayerController() { }

void PlayerController::LoadKeyBindings(/*Bindings keyBindings*/)
{
    // Temp Key Bindings

    // Movement Keys
    m_KeyMoveForward = DirectX::Keyboard::W;
    m_KeyMoveBackward = DirectX::Keyboard::S;
    m_KeyMoveLeft = DirectX::Keyboard::A;
    m_KeyMoveRight = DirectX::Keyboard::D;
    m_KeyJump = DirectX::Keyboard::Space;

    // Skill Keys
    m_KeyUseSkillPrimary = DirectX::Keyboard::E;
    m_KeyUseSkillSecondary = DirectX::Keyboard::F;
    m_KeyUseSkillTertiary = DirectX::Keyboard::G;

    // Weapon
    m_KeyWeaponReload = DirectX::Keyboard::R;
    m_KeyWeaponSwitchOne = DirectX::Keyboard::D1;
    m_KeyWeaponSwitchTwo = DirectX::Keyboard::D2;
    m_KeyWeaponSwitchThree = DirectX::Keyboard::D3;

    // Debugging
    m_KeyResetPosition = DirectX::Keyboard::B;
    m_KeyDeactivateNoClip = DirectX::Keyboard::M;
    m_KeyActivateNoClip = DirectX::Keyboard::N;
    m_KeyUpwardsNoClip = DirectX::Keyboard::LeftShift;
    m_KeyDownwardsNoClip = DirectX::Keyboard::Space;
}

void PlayerController::GetKeyInput()
{
    int directionFlag;
    if (m_KeyTracker.IsKeyPressed(m_KeyMoveLeft))       directionFlag |= DIRECTION_FLAG::DIR_LEFT;
    if (m_KeyTracker.IsKeyPressed(m_KeyMoveRight))      directionFlag |= DIRECTION_FLAG::DIR_RIGHT;
    if (m_KeyTracker.IsKeyPressed(m_KeyMoveForward))    directionFlag |= DIRECTION_FLAG::DIR_FORWARD;
    if (m_KeyTracker.IsKeyPressed(m_KeyMoveBackward))   directionFlag |= DIRECTION_FLAG::DIR_BACKWARD;

    if (m_KeyTracker.IsKeyPressed(m_KeyJump)) m_PlayerMovement->wantToJump();
    else m_PlayerMovement->doesNotWantToJump();
}

void PlayerController::GetMouseInput()
{

}

void PlayerController::Update()
{
    m_KeyTracker.Update(DirectX::Keyboard::Get().GetState());
    m_MouseTracker.Update(DirectX::Mouse::Get().GetState());

    GetKeyInput();
    GetMouseInput();
    
    if (m_KeyTracker.IsKeyPressed(m_KeyJump))
    {

    }
}