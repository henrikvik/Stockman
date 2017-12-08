#ifndef STATEPLAYING_H
#define STATEPLAYING_H

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
#include <Misc\ComboMachine.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Misc\FPSRenderer.h>
#include <Misc\GUI\iMenuMachine.h>
#include <GameType.h>

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

// Engine Includes
#include <Singletons\Profiler.h>

namespace Logic
{
    class StatePlaying : public State
    {
    private:
        struct GAME_START
        {
            static const btVector3  PLAYER_SCALE;
            static const btVector3  PLAYER_ROTATION;
            static const int        UNIQUE_CARDS;
        };

    public:
        StatePlaying(StateBuffer* stateBuffer);
        ~StatePlaying();
        void reset();

        void update(float deltaTime);
        void render() const;

        // Necessary evil
        Player* getPlayer()             { return m_player;          }
        CardManager* getCardManager()   { return m_cardManager;     } 

    private:
        void gameOver();
        void gameWon();
        void addHighscore();

        float m_playTime;
        std::thread highscoreThread;

        std::vector<LightRenderInfo> m_MapLights;
        std::vector<LightRenderInfo> m_RedBulbs;
        std::vector<LightRenderInfo> m_GreenBulbs;
        std::vector<LightRenderInfo> m_BlueBulbs;

        std::vector<char> m_ChristmasLightPattern;
        float m_ChristmasLightTimer;
        int m_ChristmasPatternIndex;

        iMenuMachine*       m_menu;
        Physics*			m_physics;
        Player*				m_player;
        Map*				m_map;
        ProjectileManager*	m_projectileManager;
        EntityManager		m_entityManager;
        WaveTimeManager		m_waveTimeManager;
        CardManager*		m_cardManager;
        HUDManager		    m_hudManager;
        GameType            m_gameType;
        FPSRenderer         m_fpsRenderer;
    };
}

#endif // !STATEPLAYING_H
