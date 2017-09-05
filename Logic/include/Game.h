#ifndef GAME_H
#define GAME_H

namespace Logic
{
	class Game
	{
	public:
		Game();
		Game(const Game& other) = delete;
		Game* operator=(const Game& other) = delete;
		~Game();

		void update();

	private:

	};
}

#endif // !GAME_H