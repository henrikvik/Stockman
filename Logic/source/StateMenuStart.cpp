#include <StateMenuStart.h>

#include <Engine\Typing.h>
#include <DebugDefines.h>
#include <Engine\DebugWindow.h> 

using namespace Logic;

StateMenuStart::StateMenuStart()
{
    // Initializing Highscore Manager
    m_highScoreManager = newd HighScoreManager();
    m_highScoreManager->setName("Stockman");

    // Initializing Menu's
    m_menu = newd MenuMachine(m_highScoreManager->getName());
    m_menu->initialize(Logic::gameStateMenuMain);

    // Getting list of highscores
    for (int i = 0; i < 10; i++)
    {
        if (m_highScoreManager->gethighScore(i).score != -1)
        {
            m_highScore[i] = std::to_string(i + 1) + ". " + m_highScoreManager->gethighScore(i).name + ": " + std::to_string(m_highScoreManager->gethighScore(i).score);
        }
        else
        {
            m_highScore[i] = "";
        }
    }
}

StateMenuStart::~StateMenuStart()
{
    m_menu->clear();

    delete m_menu;
    delete m_highScoreManager;
}

void StateMenuStart::reset()
{

}

void StateMenuStart::update(float deltaTime)
{
    m_fpsRenderer.updateFPS(deltaTime);
    m_menu->update(deltaTime);
}

void StateMenuStart::render() const
{
    m_menu->render();
}