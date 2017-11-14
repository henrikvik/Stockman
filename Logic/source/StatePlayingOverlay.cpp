#include <StatePlayingOverlay.h>
#include <StateMachine\StateBuffer.h>
#include <StateMachine\StatePrimary.h>
#include <StatePlaying.h>

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
    m_menu = newd MenuMachine();
    m_menu->initialize(Logic::gameStateSkillPick);
}

StatePlayingOverlay::~StatePlayingOverlay()
{
    m_menu->clear();
    delete m_menu;
}

void StatePlayingOverlay::reset() { }

void StatePlayingOverlay::update(float deltaTime)
{
    m_menu->update(deltaTime);
    switch (m_menu->currentState())
    {
    case gameStateSkillPick:
    {
        DirectX::Mouse::Get().SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
        std::pair<int, int>* selectedSkills = m_menu->getSkillPick();
        if (selectedSkills->first != -1 && selectedSkills->second != -1)
        {
            // Just ignore this for now, menu-machine is getting reworked.
            StatePlaying* game = (StatePlaying*)((StatePrimary*)m_stateBuffer->currentPrimaryState)->getCurrentState();
            if (game)
            {
                game->getPlayer()->setCurrentSkills(selectedSkills->first, selectedSkills->second);

                // Reset menu stuff
                selectedSkills->first = -1;
                selectedSkills->second = -1;

                m_menu->setStateToBe(gameStateGame); //change to gameStateGame
            }
        }
    }
    break;

    default:
        DirectX::Mouse::Get().SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
        break;
    }
}

void StatePlayingOverlay::render() const
{
    m_menu->render();
}