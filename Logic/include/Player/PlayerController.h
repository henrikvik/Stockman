#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <Misc\NonCopyable.h>
#include <Keyboard.h>
#include <Mouse.h>

namespace Logic
{
    class PlayerGeneral;
    class PlayerMovement;
    class PlayerController : public NonCopyable
    {
    public:
        PlayerController();
        ~PlayerController();

        void LoadKeyBindings();
        void Update();

    private:
        void GetKeyInput();
        void GetMouseInput();

        PlayerMovement* m_PlayerMovement;

        DirectX::Mouse::ButtonStateTracker m_MouseTracker;
        DirectX::Keyboard::KeyboardStateTracker m_KeyTracker;

        DirectX::Keyboard::Keys m_KeyMoveForward;
        DirectX::Keyboard::Keys m_KeyMoveBackward;
        DirectX::Keyboard::Keys m_KeyMoveLeft;
        DirectX::Keyboard::Keys m_KeyMoveRight;
        DirectX::Keyboard::Keys m_KeyJump;
        DirectX::Keyboard::Keys m_KeyUseSkillPrimary;
        DirectX::Keyboard::Keys m_KeyUseSkillSecondary;
        DirectX::Keyboard::Keys m_KeyUseSkillTertiary;
        DirectX::Keyboard::Keys m_KeyWeaponReload;
        DirectX::Keyboard::Keys m_KeyWeaponSwitchOne;
        DirectX::Keyboard::Keys m_KeyWeaponSwitchTwo;
        DirectX::Keyboard::Keys m_KeyWeaponSwitchThree;

        // Debugging purposes, remove at release
        DirectX::Keyboard::Keys m_KeyResetPosition;
        DirectX::Keyboard::Keys m_KeyDeactivateNoClip;
        DirectX::Keyboard::Keys m_KeyActivateNoClip;
        DirectX::Keyboard::Keys m_KeyUpwardsNoClip;
        DirectX::Keyboard::Keys m_KeyDownwardsNoClip;
    };
}

#endif // !PLAYERCONTROLLER_H
