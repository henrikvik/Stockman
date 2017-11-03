#ifndef GAME_H
#define GAME_H

// C++ Inlcudes
#include <stdio.h>
#include <thread>

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

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

// Graphics Includes


// Engine Includes
#include <Engine\Profiler.h>

namespace Logic
{
    enum GameType;

	class Game
	{
    private:

        struct GAME_START
        {
            static const GameState  STATE;
            static const btVector3  PLAYER_SCALE;
            static const btVector3  PLAYER_ROTATION;
            static const int        UNIQUE_CARDS;
        };

	public:
		Game();
		Game(const Game& other) = delete;
		Game* operator=(const Game& other) = delete;
		~Game();

        void init(LPWSTR *cmdLine, int args);
		void clear();
		void reset();

		void update(float deltaTime);
        bool updateMenu(float deltaTime);
		void updateGame(float deltaTime);

        void render() const;
		void renderGame() const;
		void renderMenu() const;

		DirectX::SimpleMath::Vector3 getPlayerForward();
		DirectX::SimpleMath::Vector3 getPlayerPosition();

        int getState() const;
	private:
		// Private functions
		void gameOver();

		// Members
		Physics*			m_physics;
		Player*				m_player;
		Map*				m_map;
		ProjectileManager*	m_projectileManager;
		MenuMachine*		m_menu;
        EntityManager		m_entityManager;
        HUDManager		    m_hudManager;
        WaveTimeManager		m_waveTimeManager;
		CardManager*		m_cardManager;
		HighScoreManager*	m_highScoreManager;

        GameType m_gameType;

		//GameOver
		std::string			highScore[10];

		// FPS
		FPSRenderer         m_fpsRenderer;

        // Inherited via Drawable
    };
}

#endif // !GAME_H