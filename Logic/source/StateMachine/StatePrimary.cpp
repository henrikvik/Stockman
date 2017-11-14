#include <StateMachine\StatePrimary.h>
#include <StateMachine\StateBuffer.h>
#include <StatePlaying.h>
#include <StateStart.h>

// Input Singletons
#include <Keyboard.h>
#include <Mouse.h>

// Debugging purposes
#include <DebugDefines.h>
#include <Engine\Typing.h>
#include <Engine\DebugWindow.h> 
#include <GameType.h>

using namespace Logic;

StatePrimary::StatePrimary(StateBuffer* stateBuffer)
    : State(stateBuffer)
{
    m_wantToSwitchToType = StateType::Nothing;
    m_currentStateType = StateType::Nothing;
    m_currentState = nullptr;
}

StatePrimary::~StatePrimary()
{
    if (m_currentState)
    {
        delete m_currentState;
        m_currentState = nullptr;
    }
}

void StatePrimary::reset()
{
    if (m_currentState)
        m_currentState->reset();
}

// Update the logic inside state, also checking if we should load next state
void StatePrimary::update(float deltaTime)
{
    if (m_currentState)
        m_currentState->update(deltaTime);

    if (m_wantToSwitchToType != m_currentStateType)
        loadState(m_wantToSwitchToType);
}

// Render everything in the currently active state
void StatePrimary::render() const
{
    if (m_currentState)
        m_currentState->render();
}

// Queuing a new state
void StatePrimary::queueState(StateType state)
{
    if (m_currentStateType != state &&
        m_wantToSwitchToType != state)
    {
        m_wantToSwitchToType = state;
    }
}

void StatePrimary::loadState(StateType state)
{
    // Saving the new state to a variable
    m_wantToSwitchToType = state;
    m_currentStateType = state;

    // Clear previous state from memory 
    if (m_currentState)
        delete m_currentState;

    // Reset currentstate as nullptr
    m_currentState = nullptr;

    // Inactive State
    if (m_currentStateType == StateType::Nothing)
    {
        printf("*~ Inactivated Primary State. You should prefer to not run the secondary state solo.\n");
        return;
    }

    // Stop all music & sfx
    Sound::NoiseMachine::Get().stopAllGroups();

    // Load new state to memory
    switch (state)
    {
    case StateType::State_Playing:
        m_currentState = newd StatePlaying(m_stateBuffer);
        break;
    case StateType::State_Start:
        m_currentState = newd StateStart(m_stateBuffer);
        break;
    }

    // Error check
    if (!m_currentState)
    {
        printf("*~ This state does not exist or is not valid in the primary state.");
        return;
    }

    m_currentState->SetFuncPrimarySwitch(SwitchPrimaryState);
    m_currentState->SetFuncSecondarySwitch(SwitchSecondaryState);
}