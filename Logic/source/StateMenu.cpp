#include <StateMenu.h>
#include <StateBuffer.h>
#include <StateMenuStart.h>
#include <StateMenuPlaying.h>

// Input Singletons
#include <Keyboard.h>
#include <Mouse.h>

// Debugging purposes
#include <DebugDefines.h>
#include <Engine\Typing.h>
#include <Engine\DebugWindow.h> 
#include <GameType.h>

using namespace Logic;

StateMenu::StateMenu(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    m_wantToSwitchToType = StateType::Nothing;
    m_currentStateType = StateType::Nothing;
    m_currentState = nullptr;
}

StateMenu::~StateMenu()
{
    if (m_currentState)
    {
        delete m_currentState;
        m_currentState = nullptr;
    }
}

void StateMenu::reset()
{
    if (m_currentState)
        m_currentState->reset();
}

// Update the logic inside the current state and checking if we should load a new state
void StateMenu::update(float deltaTime)
{
    if (m_currentState)
        m_currentState->update(deltaTime);

    if (m_wantToSwitchToType != m_currentStateType)
        loadState(m_wantToSwitchToType);
}

// Render the current menu state
void StateMenu::render() const
{
    if (m_currentState)
        m_currentState->render();
}

// Queing the state to be changed on the next update
void StateMenu::queueState(StateType menuState)
{
    if (m_currentStateType != menuState && 
        m_wantToSwitchToType != menuState)
    {
        m_wantToSwitchToType = menuState;
    }
}

void StateMenu::loadState(StateType menuState)
{
    // Saving the new state
    m_wantToSwitchToType = menuState;
    m_currentStateType = menuState;

    // Clear previous state from memory 
    delete m_currentState;
    m_currentState = nullptr;

    // Load new state to memory
    switch (menuState)
    {
    case StateType::Menu_Start:
        m_currentState = new StateMenuStart(m_stateBuffer);
        break;
    case StateType::Menu_Playing:
        m_currentState = new StateMenuPlaying(m_stateBuffer);
        break;
    }

    // Error check
    if (!m_currentState)
    {
        printf("This state does not exist or is not valid in this type of state-manager.");
        return;
    }

    // Saving new state in stateBuffer
    m_stateBuffer->currentMenuState = m_currentState;

    m_currentState->SetGameSwitchCallBack(SwitchParentGameState);
    m_currentState->SetMenuSwitchCallBack(SwitchParentMenuState);
    m_currentState->SetCurrentGameState(GetParentCurrentGameState);
    m_currentState->SetCurrentMenuState(GetParentCurrentMenuState);

    RenderQueue::get().clearAllQueues();
}