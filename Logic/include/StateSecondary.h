#ifndef STATEMENU_H
#define STATEMENU_H

// C++ Includes
#include <stdio.h>
#include <thread>

// State Include
#include <State.h>

namespace Logic
{
    class StateSecondary : public State
    {
    public:
        StateSecondary(StateBuffer* stateBuffer);
        ~StateSecondary();
        void reset();

        void update(float deltaTime);
        void render() const;

        void queueState(StateType state);
        void loadState(StateType state);
        State* getCurrentState() { return m_currentState; }

    private:
        StateType m_wantToSwitchToType;
        StateType m_currentStateType;
        State* m_currentState;
    };
}

#endif // !STATEMENU_H
