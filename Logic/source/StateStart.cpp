#include <StateStart.h>

#include <Engine\Typing.h>
#include <DebugDefines.h>
#include <Engine\DebugWindow.h> 

#include <Graphics\include\Renderer.h> // Remove this when henke's merge is in

using namespace Logic;

StateStart::StateStart()
{
    // Initializing Sound
    Sound::NoiseMachine::Get().init();
    Sound::ListenerData listener;
    Sound::NoiseMachine::Get().update(listener);

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
            m_highScore[i] = to_string(i + 1) + ". " + m_highScoreManager->gethighScore(i).name + ": " + to_string(m_highScoreManager->gethighScore(i).score);
        }
        else
        {
            m_highScore[i] = "";
        }
    }
}

StateStart::~StateStart()
{
    m_menu->clear();
    Sound::NoiseMachine::Get().clear();

    delete m_menu;
    delete m_highScoreManager;
}

void StateStart::reset()
{

}

void StateStart::update(float deltaTime)
{
    m_fpsRenderer.updateFPS(deltaTime);
    m_menu->update(deltaTime);

    if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::J))
        SetState(StateType::Game);
}

void StateStart::render(Graphics::Renderer& renderer)
{
    m_menu->render(renderer, m_highScore);
}

DirectX::SimpleMath::Vector3 StateStart::getCameraForward()
{
    return DirectX::SimpleMath::Vector3(0, 0, 1);
}

DirectX::SimpleMath::Vector3 StateStart::getCameraPosition()
{
    return DirectX::SimpleMath::Vector3(0, 0, 0);
}