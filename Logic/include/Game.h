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
#include <Graphics\include\Renderer.h>

// Engine Includes
#include <Engine\Profiler.h>

namespace Logic
{
    enum GameType;

	class Game
	{
    private:
        static const GameState STATE_START;
        static const btVector3 PLAYER_START_SCALE,
                               PLAYER_START_ROTATION;
        static const int NUMBER_OF_UNIQUE_CARDS;
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

		void render(Graphics::Renderer& renderer);
		void renderGame(Graphics::Renderer& renderer);
		void renderMenu(Graphics::Renderer& renderer);

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
	};
}

#endif // !GAME_H