#include "Misc\HighScoreManager.h"
#include <deque>

using namespace Logic;

HighScoreManager::HighScoreManager()
{
	for (highScore score : m_highScore)
	{
		score.score = -1;
		score.name = "???";
	}
}


HighScoreManager::~HighScoreManager()
{
}

void Logic::HighScoreManager::addNewHighScore(std::string name)
{
	highScore newScore;
	newScore.score = m_score;
	newScore.name = name;
	int ammount = sizeof(m_highScore) / sizeof(highScore);
	for (int i = 0; i < ammount; i++)
	{
		if (m_highScore[i].score < newScore.score)
		{
			highScore temp;
			temp = m_highScore[i];
			m_highScore[i] = newScore;
			newScore = temp;
		}
		else if (m_highScore[i].score == -1)
		{
			i = ammount;
		}
	}
}

void Logic::HighScoreManager::saveToFile()
{
}

void Logic::HighScoreManager::loadFromFile()
{
}


