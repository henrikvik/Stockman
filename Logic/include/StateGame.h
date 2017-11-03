#ifndef STATEGAME_H
#define STATEGAME_H

// C++ Includes
#include <stdio.h>
#include <thread>

// State Include
#include <State.h>

// Player, Physics, Map & PJM
#include <Player\Player.h>
#include <Player\HUDManager.h>
#include <Physics\Physics.h>
#include <Map.h>
#include <Projectile\ProjectileManager.h>

// AI
#include <AI/EntityManager.h>
#include <AI/WaveTimeManager.h>

// Misc
#include <Misc\CardManager.h>
#include <Misc\HighScoreManager.h>
#include <Misc\ComboMachine.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Misc\FPSRenderer.h>
#include <Misc\GUI\MenuMachine.h>
#include <GameType.h>

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
    class StateGame : public State
    {
    private:
        struct GAME_START
        {
            static const btVector3  PLAYER_SCALE;
            static const btVector3  PLAYER_ROTATION;
            static const int        UNIQUE_CARDS;
        };

    public:
        StateGame();
        ~StateGame();
        void reset();

        void update(float deltaTime);
        void render(Graphics::Renderer& renderer);

        DirectX::SimpleMath::Vector3 getCameraForward();
        DirectX::SimpleMath::Vector3 getCameraPosition();
    private:
        void gameOver();

        Physics*			m_physics;
        Player*				m_player;
        Map*				m_map;
        ProjectileManager*	m_projectileManager;
        EntityManager		m_entityManager;
        WaveTimeManager		m_waveTimeManager;
        CardManager*		m_cardManager;
        HighScoreManager*	m_highScoreManager;
        GameType            m_gameType;
        FPSRenderer         m_fpsRenderer;
    };
}

#endif // !STATEGAME_H
