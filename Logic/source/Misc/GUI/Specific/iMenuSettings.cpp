#include <Misc\GUI\Specific\iMenuSettings.h>
#include <Misc\Sound\NoiseMachine.h>
#include <Engine\Settings.h>

Logic::iMenuSettings::iMenuSettings(iMenu::MenuGroup group)
    : iMenu(group)
{
    m_textRenderInfoWindow.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    m_textRenderInfoWindow.font = Resources::Fonts::KG26;
    m_textRenderInfoWindow.position = DirectX::SimpleMath::Vector2(815.0f, 130.0f);

    m_textRenderInfoDOF.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    m_textRenderInfoDOF.font = Resources::Fonts::KG14;
    m_textRenderInfoDOF.position = DirectX::SimpleMath::Vector2(870.0f, 490.0f);

    m_textRenderInfoSSAO.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    m_textRenderInfoSSAO.font = Resources::Fonts::KG14;
    m_textRenderInfoSSAO.position = DirectX::SimpleMath::Vector2(870.0f, 515.0f);

    m_textRenderInfoFog.color = DirectX::SimpleMath::Color(1, 1, 1, 1);
    m_textRenderInfoFog.font = Resources::Fonts::KG14;
    m_textRenderInfoFog.position = DirectX::SimpleMath::Vector2(870.0f, 540.0f);
}

Logic::iMenuSettings::~iMenuSettings()
{
}

void Logic::iMenuSettings::update(int x, int y, float deltaTime)
{
    iMenu::update(x, y, deltaTime);


    if (Settings::getInstance().getWindowed() == 1)
    {
        m_textRenderInfoWindow.text = L"FULL";
        m_textRenderInfoWindow.position = DirectX::SimpleMath::Vector2(860.0f, 130.0f);
    }
    else
    {
        m_textRenderInfoWindow.text = L"WINDOWED";
        m_textRenderInfoWindow.position = DirectX::SimpleMath::Vector2(815.0f, 130.0f);
    }

    if (Settings::getInstance().getDOF() == 1)
    {
        m_textRenderInfoDOF.text = L"ON";
        m_textRenderInfoDOF.position = DirectX::SimpleMath::Vector2(880.0f, 490.0f);
    }
    else
    {
        m_textRenderInfoDOF.text = L"OFF";
        m_textRenderInfoDOF.position = DirectX::SimpleMath::Vector2(870.0f, 490.0f);
    }

    if (Settings::getInstance().getSSAO() == 1)
    {
        m_textRenderInfoSSAO.text = L"ON";
        m_textRenderInfoSSAO.position = DirectX::SimpleMath::Vector2(880.0f, 515.0f);
    }
    else
    {
        m_textRenderInfoSSAO.text = L"OFF";
        m_textRenderInfoSSAO.position = DirectX::SimpleMath::Vector2(870.0f, 515.0f);
    }

    if (Settings::getInstance().getFog() == 1)
    {
        m_textRenderInfoFog.text = L"ON";
        m_textRenderInfoFog.position = DirectX::SimpleMath::Vector2(880.0f, 540.0f);
    }   
    else
    {   
        m_textRenderInfoFog.text = L"OFF";
        m_textRenderInfoFog.position = DirectX::SimpleMath::Vector2(870.0f, 540.0f);
    }
}

void Logic::iMenuSettings::render() const
{
    iMenu::render();

    QueueRender(m_textRenderInfoWindow);
    QueueRender(m_textRenderInfoDOF);
    QueueRender(m_textRenderInfoSSAO);
    QueueRender(m_textRenderInfoFog);
}
