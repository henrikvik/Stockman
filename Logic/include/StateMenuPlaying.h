#ifndef STATEMENUPLAYING_H
#define STATEMENUPLAYING_H

// Include Abstract State
#include <State.h>

// C++ Inlcudes
#include <stdio.h>
#include <thread>

// Misc
#include <Misc\GUI\MenuMachine.h>

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

namespace Logic
{
    class StateMenuPlaying : public State
    {
    public:
        StateMenuPlaying();
        ~StateMenuPlaying();
        void reset();
        void update(float deltaTime);
        void render() const;

    private:
        MenuMachine*		m_menu;
    };
}

#endif // !STATEMENUPLAYING_H
