#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#include "Physics\Physics.h"

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
		Physics* physics;
	};
}

#endif // !GAME_H