#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <functional>
#include <Misc\NonCopyable.h>
#include <Misc\Error.h>
#include <GameType.h>

namespace Logic
{
    class State;
    class StatePrimary;
    class StateSecondary;
    struct StateBuffer;
    class StateMachine : public NonCopyable
    {
    public:
        StateMachine();
        ~StateMachine();

        void update(float deltaTime);
        void render() const;

    private:
        std::function<void(StateType stateType)>    SetPrimaryState;
        std::function<void(StateType stateType)>    SetSecondaryState;
        std::function<StatePrimary*()>              GetPrimaryState;
        std::function<StateSecondary*()>            GetSecondaryState;
        StatePrimary*                               m_statePrimary;
        StateSecondary*                             m_stateSecondary;
        StateBuffer*                                m_stateBuffer;
    };
}

#endif // !STATEMACHINE_H
