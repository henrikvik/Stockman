#ifndef GAME_H
#define GAME_H

// C++ Inlcudes
#include <stdio.h>
#include <thread>

// Logic Includes
#include <Player\Player.h>
#include <Physics\Physics.h>
#include <Map.h>
#include <Misc\GUI\MenuMachine.h>
#include <Projectile\ProjectileManager.h>
#include <AI/EntityManager.h>
#include <Misc\GameTime.h>

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

// Graphics Includes
#include <Graphics\include\Renderer.h>

// Init Defines
#define STARTING_STATE		gameStateGame
#define PLAYER_START_SCA	btVector3(1.5f, 3.0f, 1.5f)
#define PLAYER_START_POS	btVector3(0.0f, 6.0f, 0.0f)
#define PLAYER_START_ROT	btVector3(0.0f, 0.0f, 0.0f)

namespace Logic
{
	class Game
	{
	public:
		Game();
		Game(const Game& other) = delete;
		Game* operator=(const Game& other) = delete;
		~Game();

		void init();
		void clear();

		void update(float deltaTime);
		void render(Graphics::Renderer& renderer);

		DirectX::SimpleMath::Vector3 getPlayerForward();
		DirectX::SimpleMath::Vector3 getPlayerPosition();

	private:
		Physics*			m_physics;
		Player*				m_player;
		Map*				m_map;
		ProjectileManager*	m_projectileManager;
		MenuMachine*		m_menu;
		EntityManager		m_entityManager;
		GameTime			m_gameTime;
	};
}

#endif // !GAME_H