#ifndef STATEGAME_H
#define STATEGAME_H

// C++ Includes
#include <stdio.h>
#include <thread>

// State Include
#include <State.h>

namespace Logic
{
    class StateBuffer;
    class StateGame : public State
    {
    public:
        StateGame(StateBuffer* stateBuffer);
        ~StateGame();
        void reset();

        void update(float deltaTime);
        void render() const;

        void switchState(StateType gameState);
        void loadState(StateType gameState);
        State* getCurrentState() { return m_currentState; }

    private:
        StateType m_wantToSwitchToType;
        StateType m_currentStateType;
        State* m_currentState;
    };
}

#endif // !STATEGAME_H
