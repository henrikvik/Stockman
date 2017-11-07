#include <StateMachine.h>
#include <StateBuffer.h>
#include <State.h>
#include <StateGame.h>
#include <StateMenu.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Keyboard.h>

using namespace Logic;

StateMachine::StateMachine()
{
    m_stateBuffer = new StateBuffer();
    m_gameState = new StateGame(m_stateBuffer);
    m_menuState = new StateMenu(m_stateBuffer);

    // Making a function ptr to switch state inside the active state
    SetGameState = [&](StateType stateType) -> void { m_gameState->switchState(stateType); };
    SetMenuState = [&](StateType stateType) -> void { m_menuState->switchState(stateType); };
    GetGameState = [&]() -> StateGame* { return m_gameState; };
    GetMenuState = [&]() -> StateMenu* { return m_menuState; };
   
    // Save function ptr's inside each main state
    m_gameState->SetMenuSwitchCallBack  (SetMenuState);
    m_gameState->SetGameSwitchCallBack  (SetGameState);
    m_gameState->SetCurrentGameState    (GetGameState);
    m_gameState->SetCurrentMenuState    (GetMenuState);
    m_menuState->SetMenuSwitchCallBack  (SetMenuState);
    m_menuState->SetGameSwitchCallBack  (SetGameState);
    m_menuState->SetCurrentGameState    (GetGameState);
    m_menuState->SetCurrentMenuState    (GetMenuState);

    // Setting starting states
    SetMenuState(StateType::Menu_Start);
    SetGameState(StateType::Game_Start);

    // Initializing Sound
    Sound::NoiseMachine::Get().init();
    Sound::ListenerData listener;
    Sound::NoiseMachine::Get().update(listener);
}

StateMachine::~StateMachine()
{
    if (m_menuState)
    {
        delete m_menuState;
        m_menuState = nullptr;
    }

    if (m_gameState)
    {
        delete m_gameState;
        m_gameState = nullptr;
    }

    Sound::NoiseMachine::Get().clear();
}

void StateMachine::update(float deltaTime)
{
    m_gameState->update(deltaTime);
    m_menuState->update(deltaTime);

    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::NumPad1))
    {
        SetMenuState(StateType::Menu_Start);
        SetGameState(StateType::Game_Start);
        return;
    }

    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::NumPad2))
    {
        SetMenuState(StateType::Menu_Playing);
        SetGameState(StateType::Game_Playing);
        return;
    }
}

void StateMachine::render() const
{
    m_gameState->render();
    m_menuState->render();
}