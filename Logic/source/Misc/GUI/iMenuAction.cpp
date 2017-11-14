#include <Misc\GUI\iMenuAction.h>
#include <StateMachine\StateBuffer.h>
#include <StateMachine\StatePrimary.h>
#include <StateMachine\StateSecondary.h>
#include <Misc\GUI\iMenuMachine.h>
#include <Engine\Settings.h>
#include <State.h>

using namespace Logic;

void ButtonFunction::startGame()
{
    if (Action::Get().m_stateBuffer->currentPrimaryState)
        dynamic_cast<StatePrimary*>(Action::Get().m_stateBuffer->currentPrimaryState)->queueState(StateType::State_Playing);
    if (Action::Get().m_stateBuffer->currentSecondaryState)
        dynamic_cast<StateSecondary*>(Action::Get().m_stateBuffer->currentSecondaryState)->queueState(StateType::State_InGame_Overlay);
}

void ButtonFunction::startSettings()
{
    if (Action::Get().m_menuMachine)
        Action::Get().m_menuMachine->swapMenu(iMenu::MenuGroup::Settings);
}

void ButtonFunction::startMainMenu()
{
    if (Action::Get().m_menuMachine)
        Action::Get().m_menuMachine->swapMenu(iMenu::MenuGroup::Start);
}

void ButtonFunction::quitGame()
{

}

void ButtonFunction::writing()
{
}

void ButtonFunction::chooseUpgrade1()
{
}

void ButtonFunction::chooseUpgrade2()
{
}

void ButtonFunction::chooseUpgrade3()
{
}

void ButtonFunction::chooseSkill1()
{
}

void ButtonFunction::chooseSkill2()
{
}

void ButtonFunction::chooseSkill3()
{
}

void ButtonFunction::plusSense()
{
}

void ButtonFunction::minusSense()
{
}

void ButtonFunction::plusMaster()
{
}

void ButtonFunction::minusMaster()
{
}

void ButtonFunction::plusSFX()
{
}

void ButtonFunction::minusSFX()
{
}

void ButtonFunction::muteUnmute()
{
}

void ButtonFunction::plusFOV()
{
}

void ButtonFunction::minusFOV()
{
}

void ButtonFunction::windowed()
{
}

void ButtonFunction::showHighscore()
{
}