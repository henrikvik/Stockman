#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#include "Player\Player.h"
#include "Physics\Physics.h"
#include "Entity\Hitbox.h"

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
	//	void draw(RenderMachine& renderMachine);

	private:
		Physics* m_physics;
		Player* m_player;
		Hitbox* m_plane;
	};
}

#endif // !GAME_H