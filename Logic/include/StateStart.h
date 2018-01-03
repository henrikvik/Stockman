#ifndef STATESTART_H
#define STATESTART_H

// Include Abstract State
#include <State.h>

// C++ Inlcudes
#include <stdio.h>
#include <thread>

// Misc
#include <Misc\FPSRenderer.h>
#include <Misc\GUI\iMenuMachine.h>

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

// Engine Includes
#include <Singletons\Profiler.h>

// Particle
#include <Graphics\include\Particles\ParticleSystem.h>

namespace Logic
{
    class Physics;
    class Map;
    class StateStart : public State
    {
    public:
        StateStart(StateBuffer* stateBuffer);
        ~StateStart();
        void reset();
        void update(float deltaTime);
        void render() const;

    private:
        iMenuMachine*		        m_menu;
        Physics*			        m_physics;
        Map*				        m_map;
    };
}

#endif // !STATESTART_H
