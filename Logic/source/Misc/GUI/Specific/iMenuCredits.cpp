#include <Misc\GUI\Specific\iMenuCredits.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\iMenuMachine.h>
#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

iMenuCredits::iMenuCredits(iMenu::MenuGroup group)
    : iMenu(group) {  }

iMenuCredits::~iMenuCredits() { }

void iMenuCredits::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);
    credits.update(deltaTime);
}

void iMenuCredits::render() const
{
    iMenu::render();
    credits.render();
}
