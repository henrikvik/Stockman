#include <StateGame.h>
#include <StateBuffer.h>
#include <StateGameCampfire.h>
#include <StateGamePlaying.h>

// Input Singletons
#include <Keyboard.h>
#include <Mouse.h>

// Debugging purposes
#include <DebugDefines.h>
#include <Engine\Typing.h>
#include <Engine\DebugWindow.h> 
#include <GameType.h>

using namespace Logic;

StateGame::StateGame(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    m_currentState = nullptr;
}

StateGame::~StateGame()
{
    if (m_currentState)
    {
        delete m_currentState;
        m_currentState = nullptr;
    }
}

void StateGame::reset()
{
    if (m_currentState)
        m_currentState->reset();
}

void StateGame::update(float deltaTime)
{
    if (m_currentState)
        m_currentState->update(deltaTime);
}

void StateGame::render() const
{
    if (m_currentState)
        m_currentState->render();
}

void StateGame::switchState(StateType gameState)
{
    if (m_currentStateType != gameState)
    {
        // Saving the new state to a variable
        m_currentStateType = gameState;

        // Clear previous state from memory 
        if (m_currentState)
            delete m_currentState;

        // Reset currentstate as nullptr
        m_currentState = nullptr;

        // Load new state to memory
        switch (gameState)
        {
        case StateType::Game_Start:
            m_currentState = new StateGameCampfire(m_stateBuffer);
            break;
        case StateType::Game_Playing:
            m_currentState = new StateGamePlaying(m_stateBuffer);
            break;
        }

        // Error check
        if (!m_currentState)
        {
            printf("This state does not exist or is not valid in this type of state-manager.");
            return;
        }

        m_currentState->SetGameSwitchCallBack(SwitchParentGameState);
        m_currentState->SetMenuSwitchCallBack(SwitchParentMenuState);
        m_currentState->SetCurrentGameState(GetParentCurrentGameState);
        m_currentState->SetCurrentMenuState(GetParentCurrentMenuState);

        RenderQueue::get().clearAllQueues();
    }
}