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

		bool addNewHighScore(int score);
		void saveToFile();						//< Save high scores to file
		void loadFromFile();					//< Load high scores from file 
		void setName(std::string name);
		highScore gethighScore(int index);

	private:
		std::string m_name;						//< Player name
		highScore m_highScore[10];				//< List of high scores
	};
}

#endif // !HIGHSCOREMANAGER_H

