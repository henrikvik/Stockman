#include <StateMenuPlaying.h>
#include <StateBuffer.h>
#include <StateGame.h>
#include <StateGamePlaying.h>

#include <Engine\Typing.h>
#include <DebugDefines.h>
#include <Engine\DebugWindow.h> 

using namespace Logic;

StateMenuPlaying::StateMenuPlaying(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    Sound::NoiseMachine::Get().playMusic(Sound::MUSIC::MUSIC_IN_GAME, nullptr, true);

    // Initializing Menu's
    m_menu = newd MenuMachine();
    m_menu->initialize(Logic::gameStateSkillPick);
}

StateMenuPlaying::~StateMenuPlaying()
{
    m_menu->clear();
    delete m_menu;
}

void StateMenuPlaying::reset()
{
    m_menu->clear();
}

void StateMenuPlaying::update(float deltaTime)
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
                StateGamePlaying* game = dynamic_cast<StateGamePlaying*>(m_stateBuffer->currentGameState);
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

void StateMenuPlaying::render() const
{
    m_menu->render();
}