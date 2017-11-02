#include <StateMachine.h>
#include <StateGame.h>
#include <StateStart.h>

#include <Graphics\include\Renderer.h> // Remove this when merged

using namespace Logic;

#define START_STATE StateType::Game
#define MAX_STATES 2

StateMachine::StateMachine()
{
    m_currentState = nullptr;
    setState(START_STATE);
}

StateMachine::~StateMachine()
{
    // Removes the only active state
    if (m_currentState)
    {
        delete m_currentState;
        m_currentState = nullptr;
    }
}

void StateMachine::update(float deltaTime)
{
    m_currentState->update(deltaTime);
}

void StateMachine::render(Graphics::Renderer& renderer)
{
    m_currentState->render(renderer);
}

void StateMachine::setState(StateType stateType)
{
    if (stateType == Nothing)           _ERROR(_JUST_FOR_YOU)   return;
    if (int(stateType) > MAX_STATES)    _ERROR(_INVALID_INDEX)  return;
    if (int(stateType) < 1)             _ERROR(_INVALID_INDEX)  return;

    if (m_currentStateType != stateType)
    {
        // Saving the new state to a variable
        m_currentStateType = stateType;
       
        // Clear previous state from memory 
        delete m_currentState;
        m_currentState = nullptr;

        // Load new state to memory
        switch (stateType)
        {
        case Game:
            m_currentState = new StateGame();
            break;
        case Start:
            m_currentState = new StateStart();
            break;
        }
    }
}

StateType StateMachine::getCurrentState()
{
    return m_currentStateType;
}
