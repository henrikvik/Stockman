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
#include <vector>

namespace Logic
{
	class HighScoreManager
	{
	public:
		struct HighScore
		{
			int score;
			std::string name;
		};

		HighScoreManager();
		~HighScoreManager();

        // File Handling
		void saveToFile();						//< Save high scores to file
		void loadFromFile();					//< Load high scores from file 

        // highscore
		void addNewHighScore(int score);

        // sets & gets
		void setName(std::string name);
		const std::string& getName() const;

        void setMaxHighScoreSize(int size);
        int getMaxHighScoreSize() const;

		std::vector<HighScore>& getHighScores();
	private:
        static const std::string HIGHSCORE, SCORE, NAME;
        static const int MAX_HIGHSCORE_SIZE;

		std::string m_name; // name of the current player (the name that gets saved to highscore)
		std::vector<HighScore> m_highScores;
        int m_maxHighScoreSize;
	};
}

#endif // !HIGHSCOREMANAGER_H

