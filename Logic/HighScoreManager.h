#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>

namespace Logic
{
	class HighScoreManager
	{
	public:
		HighScoreManager();
		~HighScoreManager();

		void saveToFile();
		void loadFromFile();
	private:
		int m_score;
		int m_highScore[10];
		std::string m_highScoreNames[10];
		float m_comboTimer;
		int m_comboCount;
		int m_killCount;
	};
}

#endif // !HIGHSCOREMANAGER_H

