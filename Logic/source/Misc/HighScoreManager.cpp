#include "Misc\HighScoreManager.h"
#include <Misc\FileLoader.h>

using namespace Logic;

HighScoreManager::HighScoreManager()
{
	int ammount = sizeof(m_highScore) / sizeof(highScore);
	for (int i = 0; i < ammount; i++)
	{
		m_highScore[i].score = -1;
		m_highScore[i].name = "???";
	}
	m_score = 0;
	m_name = "";

	/*loadFromFile();*/
}


HighScoreManager::~HighScoreManager()
{
	saveToFile();
}

bool Logic::HighScoreManager::addNewHighScore()
{
	highScore newScore;
	newScore.score = m_score;
	newScore.name = m_name;
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
		FileLoader::LoadedStruct tempSave;
		tempSave.floats["Score"] = m_highScore[i].score;
		tempSave.strings["Player"] = m_highScore[i].name;
		saveTo.push_back(tempSave);
	}
	FileLoader::singleton().saveStructsToFile(saveTo, "Highscore");
}

void Logic::HighScoreManager::loadFromFile()
{
	std::vector<FileLoader::LoadedStruct> loadTo;
	FileLoader::singleton().loadStructsFromFile(loadTo, "Highscore");
	int i = 0;

	for (auto const& theScore : loadTo)
	{
		m_highScore[i].score = theScore.floats.at("Score");
		m_highScore[i].name = theScore.strings.at("Player");
		i++;
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

void  Logic::HighScoreManager::setName(std::string name)
{
	m_name = name;
}



