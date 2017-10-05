#include "Misc\HighScoreManager.h"
#include <Misc\FileLoader.h>

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

bool Logic::HighScoreManager::addNewHighScore(std::string name)
{
	highScore newScore;
	newScore.score = m_score;
	newScore.name = name;
	int ammount = sizeof(m_highScore) / sizeof(highScore);
	bool isNewHighScore = false;
	for (int i = 0; i < ammount; i++)
	{
		if (m_highScore[i].score < newScore.score)
		{
			highScore temp;
			temp = m_highScore[i];
			m_highScore[i] = newScore;
			newScore = temp;
			isNewHighScore = true;
		}
		else if (newScore.score == -1)
		{
			return isNewHighScore;
		}
	}
	return isNewHighScore;
}

void Logic::HighScoreManager::saveToFile()
{
	std::vector<FileLoader::LoadedStruct> saveTo;
	int ammount = sizeof(m_highScore) / sizeof(highScore);
	for (int i = 0; i < ammount; i++)
	{
		saveTo.at(0).floats["Score" + i] = m_highScore[i].score;
		saveTo.at(0).strings["Player" + i] = m_highScore[i].name;
	}
	FileLoader::singleton().saveStructsToFile(saveTo, "Highscore");
}

void Logic::HighScoreManager::loadFromFile()
{
	std::vector<FileLoader::LoadedStruct> loadTo;
	FileLoader::singleton().saveStructsToFile(loadTo, "Highscore");
	int ammount = sizeof(m_highScore) / sizeof(highScore);

	for (int i = 0; i < ammount; i++)
	{
		m_highScore[i].score = loadTo.at(0).floats["Score" + i];
		m_highScore[i].name = loadTo.at(0).strings["Player" + i];
	}
}

void Logic::HighScoreManager::deadCount()
{
	m_killCount++;
	m_comboCount++;
	m_score += 1 * m_comboCount;
	m_comboTimer = 5;
}

void Logic::HighScoreManager::comboCheck(float dt)
{
	if (m_comboTimer > 0)
	{
		m_comboTimer = m_comboTimer - dt;
	}
	else if(m_comboCount != 0)
	{
		m_comboCount = 0;
	}
}




