#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#pragma region Comment

/*
	Class: HighScoreManager
	Author: Emanuel Bjurman

	Description:
	This class handles high scores.
*/

#pragma endregion Description of class

#include <string>

namespace Logic
{
	class HighScoreManager
	{
	public:
		HighScoreManager();
		~HighScoreManager();

		void saveToFile();						//< Save high scores to file
		void loadFromFile();					//< Load high scores from file 
	private:
		int m_score;							//< Current score
		int m_highScore[10];					//< List of high scores
		std::string m_highScoreNames[10];		//< List of names associated with the high scores
		float m_comboTimer;						//< Current combo timer
		int m_comboCount;						//< Killcount for current combo/streak
		int m_killCount;						//< Overall killcount for current game
	};
}

#endif // !HIGHSCOREMANAGER_H

