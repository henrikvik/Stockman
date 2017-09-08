#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#include "Player\Player.h"
#include "Physics\Physics.h"

#include <Windows.h>
#include "Keyboard.h"

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

		void update(float deltaTime, DirectX::Keyboard::State* mKeyboard);
	//	void draw(RenderMachine& renderMachine);

	private:
		Physics* m_physics;
		Player* m_player;
	};
}

#endif // !GAME_H