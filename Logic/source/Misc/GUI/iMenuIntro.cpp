#include <Misc\GUI\iMenuIntro.h>
#include <Keyboard.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\iMenuMachine.h>
#include <Engine\Settings.h>
#include <Misc\Sound\NoiseMachine.h>

#define QUICK_INTO_GAME false
#if QUICK_INTO_GAME
#include <State.h>
#include <StateMachine\StateBuffer.h>
#include <StateMachine\StatePrimary.h>
#include <StateMachine\StateSecondary.h>
#include <StatePlaying.h>
#endif

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
        Sound::NoiseMachine::Get().playSFX(Sound::SFX::UI_BUTTON_PRESS, nullptr, true);

        std::string name = Settings::getInstance().getName();
        if (name.empty())   Action::Get().m_menuMachine->queueMenu(iMenu::MenuGroup::FirstTime);
        else                Action::Get().m_menuMachine->queueMenu(iMenu::MenuGroup::Start);

        #if QUICK_INTO_GAME
        printf("Skipping menu's, disable this is: %s", __FILE__);
        if (Action::Get().m_stateBuffer->currentPrimaryState) if (StatePrimary* primary = dynamic_cast<StatePrimary*>(Action::Get().m_stateBuffer->currentPrimaryState))primary->queueState(StateType::State_Playing);
        if (Action::Get().m_stateBuffer->currentSecondaryState) if (StateSecondary* secondary = dynamic_cast<StateSecondary*>(Action::Get().m_stateBuffer->currentSecondaryState))secondary->queueState(StateType::Nothing);
        #endif
    }
}
