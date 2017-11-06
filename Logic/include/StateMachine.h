#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <functional>
#include <Misc\NonCopyable.h>
#include <Misc\Error.h>
#include <GameType.h>

namespace Logic
{
    class StateGame;
    class StateMenu;
    class StateMachine : public NonCopyable
    {
    public:
        StateMachine();
        ~StateMachine();

        void update(float deltaTime);
        void render() const;

    private:
        std::function<void(StateType stateType)>    SetGameState;
        std::function<void(StateType stateType)>    SetMenuState;
        std::function<StateGame*()>                 GetGameState;
        std::function<StateMenu*()>                 GetMenuState;
        StateGame*                                  m_gameState;
        StateMenu*                                  m_menuState;
    };
}

#endif // !STATEMACHINE_H
