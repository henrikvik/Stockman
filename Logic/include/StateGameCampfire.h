#ifndef STATEGAMECAMPFIRE_H
#define STATEGAMECAMPFIRE_H

// C++ Includes
#include <stdio.h>
#include <thread>

// State Include
#include <State.h>

// To Build the little campfire
#include <Physics\Physics.h>
#include <Map.h>

// Misc
#include <Misc\Sound\NoiseMachine.h>
#include <Misc\GUI\MenuMachine.h>

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

// Engine Includes
#include <Engine\Profiler.h>

namespace Logic
{
    class StateGameCampfire : public State
    {
    public:
        StateGameCampfire();
        ~StateGameCampfire();
        void reset();

        void update(float deltaTime);
        void render() const;

    private:
        Physics*			m_physics;
        Map*				m_map;
        GameType            m_gameType;
    };
}

#endif // !STATEGAMECAMPFIRE_H