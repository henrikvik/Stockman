#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Misc\NonCopyable.h>
#include <Misc\Error.h>
#include <State.h>
#include <functional>

namespace Graphics { class Renderer; }
namespace Logic
{
    class StateMachine : public NonCopyable
    {
    public:
        StateMachine();
        ~StateMachine();

        void update(float deltaTime);
        void render() const;

        void switchState(StateType stateType);
        StateType getCurrentStateType();
        State* getState();

    private:
        std::function<void(StateType stateType)> SetStateFunction;
        StateType m_currentStateType;
        State* m_currentState;
    };
}

#endif // !STATEMACHINE_H
