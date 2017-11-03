#ifndef STATESTART_H
#define STATESTART_H

// Include Abstract State
#include <State.h>

// C++ Inlcudes
#include <stdio.h>
#include <thread>

// Misc
#include <Misc\HighScoreManager.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Misc\FPSRenderer.h>
#include <Misc\GUI\MenuMachine.h>

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

// Graphics Includes
#include <Graphics\include\Renderer.h>

// Engine Includes
#include <Engine\Profiler.h>

namespace Logic
{
    class StateStart : public State
    {
    public:
        StateStart();
        ~StateStart();
        void reset();
        void update(float deltaTime);
        void render(Graphics::Renderer& renderer);

        DirectX::SimpleMath::Vector3 getCameraForward();
        DirectX::SimpleMath::Vector3 getCameraPosition();

    private:
        std::string			m_highScore[10];
        MenuMachine*		m_menu;
        HighScoreManager*	m_highScoreManager;
        FPSRenderer         m_fpsRenderer;
    };
}

#endif // !STATEGAME_H
