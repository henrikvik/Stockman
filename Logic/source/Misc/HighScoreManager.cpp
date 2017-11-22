#include "Misc\HighScoreManager.h"
#include <Misc\FileLoader.h>

using namespace Logic;

const std::string HighScoreManager::HIGHSCORE = "Highscore",
                         HighScoreManager:: SCORE     = "Score", 
                         HighScoreManager::NAME      = "Name";
const int HighScoreManager::MAX_HIGHSCORE_SIZE = 10;

HighScoreManager::HighScoreManager()
{
	m_name = "";
    m_maxHighScoreSize = MAX_HIGHSCORE_SIZE;

	loadFromFile();
}

HighScoreManager::~HighScoreManager()
{
	saveToFile();
}

void HighScoreManager::addNewHighScore(int score)
{
    m_highScores.push_back({ score, m_name });

    // basicly bubble sort
    for (int i = static_cast<int> (m_highScores.size()) - 2; i >= 0; i--)
        if (m_highScores[i].score < score)
            std::swap(m_highScores[i], m_highScores[i + 1]);

    if (m_highScores.size() > m_maxHighScoreSize)
        m_highScores.pop_back();
}

void HighScoreManager::saveToFile()
{
	std::vector<FileLoader::LoadedStruct> save;
	FileLoader::LoadedStruct scoreSave;
	for (auto &score : m_highScores)
	{
        scoreSave.ints[SCORE]     = score.score;
        scoreSave.strings[NAME]   = score.name;
        save.push_back(scoreSave);
	}
	FileLoader::singleton().saveStructsToFile(save, HIGHSCORE);
}

void HighScoreManager::loadFromFile()
{
	std::vector<FileLoader::LoadedStruct> loadTo;
	FileLoader::singleton().loadStructsFromFile(loadTo, HIGHSCORE);
    m_highScores.clear();

	for (auto const& theScore : loadTo)
//        m_highScores.push_back({ theScore.ints.at(SCORE), theScore.strings.at(NAME) });

    if (m_maxHighScoreSize < m_highScores.size()) m_maxHighScoreSize = static_cast<int> (m_highScores.size());
}

void HighScoreManager::setName(std::string name)
{
	m_name = name;
}

const std::string& HighScoreManager::getName() const
{
	return m_name;
}

void HighScoreManager::setMaxHighScoreSize(int size)
{
    m_maxHighScoreSize = size;
}

std::vector<HighScoreManager::HighScore>& HighScoreManager::getHighScores()
{
    return m_highScores;
}

int HighScoreManager::getMaxHighScoreSize() const
{
    return m_maxHighScoreSize;
}