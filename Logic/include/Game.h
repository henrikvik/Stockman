#ifndef GAME_H
#define GAME_H

#include <stdio.h>

// Logic Includes
#include <Player\Player.h>
#include <Physics\Physics.h>
#include <Misc\RenderRegister.h>
#include <Map.h>

// DirectX Includes
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

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
		void render();

		std::queue<Graphics::RenderInfo*>* getRenderQueue();
	
		DirectX::SimpleMath::Vector3 getPlayerForward();
		DirectX::SimpleMath::Vector3 getPlayerPosition();

	private:
		RenderRegister		m_register;
		Physics*			m_physics;
		Player*				m_player;
		Map*				m_map;
	};
}

#endif // !GAME_H