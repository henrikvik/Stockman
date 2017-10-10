#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#pragma region Comment

/*
	Class: HighScoreManager
	Author:

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
		struct highScore
		{
			int score;
			std::string name;
		};
		HighScoreManager();
		~HighScoreManager();

		bool addNewHighScore();
		void saveToFile();						//< Save high scores to file
		void loadFromFile();					//< Load high scores from file 
		void deadCount();
		void comboCheck(float dt);
		void setName(std::string name);
		highScore gethighScore(int index);

	private:


		int m_score;							//< Current score
		std::string m_name;						//< Player name
		highScore m_highScore[10];				//< List of high scores
		float m_comboTimer;						//< Current combo timer
		int m_comboCount;						//< Killcount for current combo/streak
		int m_killCount;						//< Overall killcount for current game
	};
}

#endif // !HIGHSCOREMANAGER_H

