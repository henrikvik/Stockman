#include <StatePlayingOverlay.h>
#include <StateMachine\StateBuffer.h>
#include <StateMachine\StatePrimary.h>
#include <StatePlaying.h>
#include <Misc\GUI\iMenu.h>

#include <Engine\Typing.h>
#include <DebugDefines.h>
#include <Engine\DebugWindow.h> 

using namespace Logic;

StatePlayingOverlay::StatePlayingOverlay(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::AMBIENT_STORM, nullptr, true);
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::MUSIC_IN_GAME, nullptr, true);

    // Initializing Menu's
    m_menu = newd iMenuMachine();
    m_menu->swapMenu(iMenu::MenuGroup::Skill);
}

StatePlayingOverlay::~StatePlayingOverlay()
{
    delete m_menu;
}

void StatePlayingOverlay::reset() { }

void StatePlayingOverlay::update(float deltaTime)
{
    m_menu->update(deltaTime);
}

void StatePlayingOverlay::render() const
{
    m_menu->render();
}