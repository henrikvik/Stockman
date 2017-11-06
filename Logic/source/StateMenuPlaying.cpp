#include <StateMenuPlaying.h>
#include <StateGame.h>
#include <StateBuffer.h>
#include <StateGamePlaying.h>
#include <Player\Skill\SkillManager.h>

#include <Engine\Typing.h>
#include <DebugDefines.h>
#include <Engine\DebugWindow.h> 

using namespace Logic;

StateMenuPlaying::StateMenuPlaying(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    // Initializing Menu's
    m_menu = newd MenuMachine();
    m_menu->initialize(Logic::gameStateSkillPick);
  
    int testBufferSend = 5;
    stateBuffer->SendBuffer((void*)&testBufferSend, BufferType::Int, StateType::Menu_Playing, StateType::Game_Playing);
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
                StateGamePlaying* playingState = static_cast<StateGamePlaying*>(GetParentCurrentGameState()->getCurrentState());
                if (playingState)
                {
                    playingState->getPlayer()->getSkillManager()->switchToSkill(
                    {
                        SkillManager::SKILL(selectedSkills->second),
                        SkillManager::SKILL(selectedSkills->first)
                    });
                    playingState->getPlayer()->setCurrentSkills(selectedSkills->first, selectedSkills->second);

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