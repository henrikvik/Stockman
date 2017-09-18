#ifndef GAME_H
#define GAME_H

#include <stdio.h>

// Logic Includes
#include <Player\Player.h>
#include <Physics\Physics.h>
#include <Map.h>

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

// Graphics Includes
#include <Graphics\include\Renderer.h>
#include <AI/EntityManager.h>
#include <thread>

namespace Logic
{
	class Game
	{
	public:
		Game();
		Game(const Game& other) = delete;
		Game* operator=(const Game& other) = delete;
		~Game();

		bool init();
		void clear();

		void update(float deltaTime);
		void render(Graphics::Renderer& renderer);

		DirectX::SimpleMath::Vector3 getPlayerForward();
		DirectX::SimpleMath::Vector3 getPlayerPosition();

	private:
		Physics*			m_physics;
		Player*				m_player;
		Map*				m_map;
	};
}

#endif // !GAME_H