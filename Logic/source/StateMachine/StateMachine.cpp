#include <State.h>

#include <StateMachine/StateMachine.h>
#include <StateMachine/StateBuffer.h>
#include <StateMachine/StatePrimary.h>
#include <StateMachine/StateSecondary.h>

#include <Misc\Sound\NoiseMachine.h>
#include <Misc\GUI\iMenuAction.h>

#include <Keyboard.h>

using namespace Logic;

#define SKIP_MENUS true

StateMachine::StateMachine()
{
    m_stateBuffer = newd StateBuffer();
    m_statePrimary = newd StatePrimary(m_stateBuffer);
    m_stateSecondary = newd StateSecondary(m_stateBuffer);
    m_stateBuffer->currentPrimaryState = m_statePrimary;
    m_stateBuffer->currentSecondaryState = m_stateSecondary;
    Action::Get().SetPointer(m_stateBuffer);

    // Making a function ptr to switch state inside the active state
    SetPrimaryState = [&](StateType stateType) -> void { m_statePrimary->queueState(stateType); };
    SetSecondaryState = [&](StateType stateType) -> void { m_stateSecondary->queueState(stateType); };
    GetPrimaryState = [&]() -> StatePrimary* { return m_statePrimary; };
    GetSecondaryState = [&]() -> StateSecondary* { return m_stateSecondary; };
   
    // Save function ptr's inside each main state
    m_statePrimary->SetFuncSecondarySwitch      (SetSecondaryState);
    m_statePrimary->SetFuncPrimarySwitch        (SetPrimaryState);
    m_stateSecondary->SetFuncSecondarySwitch    (SetSecondaryState);
    m_stateSecondary->SetFuncPrimarySwitch      (SetPrimaryState);

    // Setting starting states
    SetPrimaryState(StateType::State_Start);
    SetSecondaryState(StateType::Nothing);

    // Initializing Sound
    Sound::NoiseMachine::Get().init();
    Sound::ListenerData listener;
    Sound::NoiseMachine::Get().update(listener);

    /*
    //
    //
    // Fast Skip (Debugging) */
 
#if !(SKIP_MENUS)
    SetPrimaryState(StateType::State_Playing);
    SetSecondaryState(StateType::Nothing);
#endif
}

StateMachine::~StateMachine()
{
    if (m_statePrimary)
    {
        delete m_statePrimary;
        m_statePrimary = nullptr;
    }

    if (m_stateSecondary)
    {
        delete m_stateSecondary;
        m_stateSecondary = nullptr;
    }

    if (m_stateBuffer)
    {
        delete m_stateBuffer;
        m_stateBuffer = nullptr;
    }

    Sound::NoiseMachine::Get().clear();
}

bool StateMachine::update(float deltaTime)
{
    m_statePrimary->update(deltaTime);
    m_stateSecondary->update(deltaTime);

#ifdef _DEBUG
    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::NumPad1))
    {
        SetPrimaryState(StateType::State_Start);
        SetSecondaryState(StateType::Nothing);
        return false;
    }

    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::NumPad2))
    {
        SetPrimaryState(StateType::State_Playing);
        SetSecondaryState(StateType::Nothing);
        return false;
    }
#endif // !_DEBUG

    return m_statePrimary->getShouldQuit();
}

void StateMachine::render() const
{
    m_statePrimary->render();
    m_stateSecondary->render();
}