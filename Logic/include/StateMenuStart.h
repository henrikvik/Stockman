#ifndef STATEMENUSTART_H
#define STATEMENUSTART_H

// Include Abstract State
#include <State.h>

// C++ Inlcudes
#include <stdio.h>
#include <thread>

// Misc
#include <Misc\HighScoreManager.h>
#include <Misc\FPSRenderer.h>
#include <Misc\GUI\MenuMachine.h>

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

// Engine Includes
#include <Engine\Profiler.h>

namespace Logic
{
    class StateMenuStart : public State
    {
    public:
        StateMenuStart();
        ~StateMenuStart();
        void reset();
        void update(float deltaTime);
        void render() const;

    private:
        std::string			m_highScore[10];
        MenuMachine*		m_menu;
        HighScoreManager*	m_highScoreManager;
        FPSRenderer         m_fpsRenderer;
    };
}

#endif // !STATEMENUSTART_H
