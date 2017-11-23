#ifndef STATEPRIMARY_H
#define STATEPRIMARY_H

// C++ Includes
#include <stdio.h>
#include <thread>

// State Include
#include <State.h>

namespace Logic
{
    class StatePrimary : public State
    {
    public:
        StatePrimary(StateBuffer* stateBuffer);
        ~StatePrimary();
        void reset();

        void update(float deltaTime);
        void render() const;

        void queueState(StateType state);
        void loadState(StateType state);
        State* getCurrentState() { return m_currentState; }
        bool getShouldQuit()const;
        void setQuit();

    private:
        StateType m_wantToSwitchToType;
        StateType m_currentStateType;
        State* m_currentState;
        bool m_quit;
    };
}

#endif // !STATEPRIMARY_H
