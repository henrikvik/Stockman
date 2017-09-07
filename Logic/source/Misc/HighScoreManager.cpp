#include "Misc\HighScoreManager.h"

using namespace Logic;

HighScoreManager::HighScoreManager()
{
	for (int score : m_highScore)
		score = 0;
	for (std::string name : m_highScoreNames)
		name = "unknown";
}


HighScoreManager::~HighScoreManager()
{
}

void Logic::HighScoreManager::saveToFile()
{
}

void Logic::HighScoreManager::loadFromFile()
{
}
