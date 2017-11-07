#ifndef STATEGAMEPLAYING_H
#define STATEGAMEPLAYING_H

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

// Engine Includes
#include <Engine\Profiler.h>

namespace Logic
{
    class StateGamePlaying : public State
    {
    private:
        struct GAME_START
        {
            static const btVector3  PLAYER_SCALE;
            static const btVector3  PLAYER_ROTATION;
            static const int        UNIQUE_CARDS;
        };

    public:
        StateGamePlaying(StateBuffer* stateBuffer);
        ~StateGamePlaying();
        void reset();

        void update(float deltaTime);
        void render() const;

        Player* getPlayer() { return m_player; }

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
        HUDManager		    m_hudManager;
        GameType            m_gameType;
        FPSRenderer         m_fpsRenderer;
    };
}

#endif // !STATEGAMEPLAYING_H
