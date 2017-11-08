#include <StateMachine.h>
#include <StateBuffer.h>
#include <State.h>
#include <StatePrimary.h>
#include <StateSecondary.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Keyboard.h>

using namespace Logic;

StateMachine::StateMachine()
{
    m_stateBuffer = new StateBuffer();
    m_statePrimary = new StatePrimary(m_stateBuffer);
    m_stateSecondary = new StateSecondary(m_stateBuffer);

    // Making a function ptr to switch state inside the active state
    SetPrimaryState = [&](StateType stateType) -> void { m_statePrimary->queueState(stateType); };
    SetSecondaryState = [&](StateType stateType) -> void { m_stateSecondary->queueState(stateType); };
    GetPrimaryState = [&]() -> StatePrimary* { return m_statePrimary; };
    GetSecondaryState = [&]() -> StateSecondary* { return m_stateSecondary; };
   
    // Save function ptr's inside each main state
    m_statePrimary->SetFuncSecondarySwitch      (SetSecondaryState);
    m_statePrimary->SetFuncPrimarySwitch        (SetPrimaryState);
    m_statePrimary->SetFuncGetCurrentPrimary    (GetPrimaryState);
    m_statePrimary->SetFuncGetCurrentSecondary  (GetSecondaryState);
    m_stateSecondary->SetFuncSecondarySwitch    (SetSecondaryState);
    m_stateSecondary->SetFuncPrimarySwitch      (SetPrimaryState);
    m_stateSecondary->SetFuncGetCurrentPrimary  (GetPrimaryState);
    m_stateSecondary->SetFuncGetCurrentSecondary(GetSecondaryState);

    // Setting starting states
    SetPrimaryState(StateType::Game_Start);
    SetSecondaryState(StateType::Menu_Start);

    // Initializing Sound
    Sound::NoiseMachine::Get().init();
    Sound::ListenerData listener;
    Sound::NoiseMachine::Get().update(listener);

    /*
    //
    //
    // Fast Skip (Debugging) */
    SetPrimaryState(StateType::Game_Playing);
    SetSecondaryState(StateType::Menu_Playing);
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

void StateMachine::update(float deltaTime)
{
    m_statePrimary->update(deltaTime);
    m_stateSecondary->update(deltaTime);

    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::NumPad1))
    {
        SetPrimaryState(StateType::Game_Start);
        SetSecondaryState(StateType::Menu_Start);
        return;
    }

    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::NumPad2))
    {
        SetPrimaryState(StateType::Game_Playing);
        SetSecondaryState(StateType::Menu_Playing);
        return;
    }
}

void StateMachine::render() const
{
    m_statePrimary->render();
    m_stateSecondary->render();
}