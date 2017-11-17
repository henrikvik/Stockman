#include <Misc\GUI\iMenuIntro.h>
#include <Keyboard.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\iMenuMachine.h>

using namespace Logic;

iMenuIntro::iMenuIntro(iMenu::MenuGroup group)
    : iMenu(group) {  }

iMenuIntro::~iMenuIntro() { }

void iMenuIntro::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);

    // Lets the player press space to go to the menu
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::Space) && !m_isFading && !m_safeToRemove)
    {
        Action::Get().m_menuMachine->queueMenu(iMenu::MenuGroup::Start);
    }
}
