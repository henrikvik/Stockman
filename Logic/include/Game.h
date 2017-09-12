#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#include "Player\Player.h"
#include "Physics\Physics.h"
#include "Entity\Hitbox.h"
#include "Misc\RenderRegister.h"

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

	private:
		RenderRegister m_register;
		Physics* m_physics;
		Player* m_player;
		Hitbox* m_plane;
	};
}

#endif // !GAME_H