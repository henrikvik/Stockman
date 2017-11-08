#include <StateSecondary.h>
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

StateSecondary::StateSecondary(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    m_wantToSwitchToType = StateType::Nothing;
    m_currentStateType = StateType::Nothing;
    m_currentState = nullptr;
}

StateSecondary::~StateSecondary()
{
    if (m_currentState)
    {
        delete m_currentState;
        m_currentState = nullptr;
    }
}

void StateSecondary::reset()
{
    if (m_currentState)
        m_currentState->reset();
}

// Update the logic inside the current state and checking if we should load a new state
void StateSecondary::update(float deltaTime)
{
    if (m_currentState)
        m_currentState->update(deltaTime);

    if (m_wantToSwitchToType != m_currentStateType)
        loadState(m_wantToSwitchToType);
}

// Render the current menu state
void StateSecondary::render() const
{
    if (m_currentState)
        m_currentState->render();
}

// Queing the state to be changed on the next update
void StateSecondary::queueState(StateType state)
{
    if (m_currentStateType != state &&
        m_wantToSwitchToType != state)
    {
        m_wantToSwitchToType = state;
    }
}

void StateSecondary::loadState(StateType state)
{
    // Saving the new state
    m_wantToSwitchToType = state;
    m_currentStateType = state;

    // Clear previous state from memory 
    delete m_currentState;
    m_currentState = nullptr;

    // Load new state to memory
    switch (state)
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
    m_stateBuffer->currentSecondaryState = m_currentState;

    m_currentState->SetFuncPrimarySwitch(SwitchPrimaryState);
    m_currentState->SetFuncSecondarySwitch(SwitchSecondaryState);
    m_currentState->SetFuncGetCurrentPrimary(GetCurrentPrimaryState);
    m_currentState->SetFuncGetCurrentSecondary(GetCurrentSecondaryState);

    RenderQueue::get().clearAllQueues();
}