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
    m_menu = newd MenuMachine();
    m_menu->initialize(Logic::gameStateMenuMain);
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