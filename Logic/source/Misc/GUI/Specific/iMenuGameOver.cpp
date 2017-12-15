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
    m_pointsStr = std::to_wstring(score);

    m_textPointsInfo.color = DirectX::SimpleMath::Color(0.f, 0.f, 0.f, 0.f);
    m_textPointsInfo.font = Resources::Fonts::KG14;
    m_textPointsInfo.isMoveable = true;
    m_textPointsInfo.position = DirectX::SimpleMath::Vector2(80, 14);
    m_textPointsInfo.text = m_pointsStr.c_str();

    m_scoreTexture.alpha = 0.f;
    m_scoreTexture.isMoveable = true;
    m_scoreTexture.screenRect = FloatRect(60.f / WIN_WIDTH, 15.f / WIN_WIDTH, 170.f / WIN_WIDTH, 60.f / WIN_WIDTH);
    m_scoreTexture.texture = Resources::Textures::Gamesheet;
    m_scoreTexture.textureRect = FloatRect(26.f / 1024.f, 24.f / 1024.f, 364.0f / 1024.f, 85.0f / 1024.f);
}

void iMenuGameOver::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);
        
    float alpha = m_fader.getCurrentPercentage();
    m_scoreTexture.alpha = alpha;
    m_textPointsInfo.color = DirectX::SimpleMath::Color(alpha, alpha, alpha, alpha);
}

void iMenuGameOver::render() const
{
    iMenu::render();

    QueueRender(m_scoreTexture);
    QueueRender(m_textPointsInfo);
}