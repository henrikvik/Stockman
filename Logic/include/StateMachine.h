#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Misc\NonCopyable.h>
#include <Misc\Error.h>

namespace Graphics { class Renderer; }
namespace Logic
{
    enum StateType
    {
        Nothing     = 0,
        Start       = 1,
        Game        = 2
    };

    class StateMachine : NonCopyable
    {
    public:
        StateMachine();
        ~StateMachine();

        void update(float deltaTime);
        void render(Graphics::Renderer& renderer);

        void setState(StateType stateType);
        StateType getCurrentState();

    private:
        StateType m_currentStateType;
        State* m_currentState;
    };


}

#endif // !STATEMACHINE_H
