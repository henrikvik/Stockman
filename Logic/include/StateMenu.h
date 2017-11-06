#ifndef STATEMENU_H
#define STATEMENU_H

// C++ Includes
#include <stdio.h>
#include <thread>

// State Include
#include <State.h>

namespace Logic
{
    class StateMenu : public State
    {
    public:

        StateMenu();
        ~StateMenu();
        void reset();

        void update(float deltaTime);
        void render() const;

        void switchState(StateType menuState);
        State* getCurrentState() { return m_currentState; }

    private:
        StateType m_currentStateType;
        State* m_currentState;
    };
}

#endif // !STATEMENU_H
