#include <Misc\GUI\Specific\iMenuControls.h>
#include <Misc\GUI\iMenuMachine.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Keyboard.h>

using namespace Logic;

iMenuControls::iMenuControls(iMenu::MenuGroup group)
    : iMenu(group) {  }

void iMenuControls::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    if (m_isFading) return;

    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::F1) ||
        DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Escape))
    {
        Sound::NoiseMachine::Get().playSFX(Sound::SFX::UI_BUTTON_PRESS, nullptr, true);
        Action::Get().m_menuMachine->queueMenu(iMenu::Empty);
    }
}