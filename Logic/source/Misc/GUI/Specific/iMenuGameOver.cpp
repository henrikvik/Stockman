#include <Misc\GUI\Specific\iMenuGameOver.h>
#include <Keyboard.h>
#include <Misc\GUI\iMenuAction.h>
#include <Misc\GUI\iMenuMachine.h>
#include <Engine\Settings.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Misc\ComboMachine.h>

using namespace Logic;

iMenuGameOver::iMenuGameOver(iMenu::MenuGroup group)
    : iMenu(group) 
{
    int score = ComboMachine::Get().getTotalScore();
    m_pointsStr = L"SCORE: " + std::to_wstring(score);

    m_textPointsInfo.color = DirectX::SimpleMath::Color(0.f, 0.f, 0.f, 0.f);
    m_textPointsInfo.font = Resources::Fonts::KG14;
    m_textPointsInfo.isMoveable = true;
    m_textPointsInfo.position = DirectX::SimpleMath::Vector2(10.f, 10.f);
    m_textPointsInfo.text = m_pointsStr.c_str();
}

iMenuGameOver::~iMenuGameOver() { }

void iMenuGameOver::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);
        
    float alpha = m_fader.getCurrentPercentage();
    m_textPointsInfo.color = DirectX::SimpleMath::Color(alpha, alpha, alpha, alpha);
}

void iMenuGameOver::render() const
{
    iMenu::render();

    QueueRender(m_textPointsInfo);
}