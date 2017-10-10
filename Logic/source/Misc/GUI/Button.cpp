#include "..\..\..\include\Misc\GUI\Button.h"

Logic::Button::Button()
{
}

Logic::Button::~Button()
{
}

void Logic::Button::initialize(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 texCoordStart, DirectX::SimpleMath::Vector2 texCoordEnd, float offset, float height, float width, std::string texture, std::function<void(void)> callBack)
{
	buttonInfo.m_rek = DirectX::SimpleMath::Rectangle(pos.x, pos.y, width, height);
	m_animationStart = DirectX::SimpleMath::Vector2(pos.x, pos.y);
	m_animationEnd = DirectX::SimpleMath::Vector2(0 - width, pos.y);
	m_animationTime = 0;
	buttonInfo.m_texCoordStart = texCoordStart;
	buttonInfo.m_texCoordEnd = texCoordEnd;
    buttonInfo.activeoffset = offset;
	buttonInfo.m_texture = texture;
	m_CallBack = callBack;
    highlighted = false;
    start = buttonInfo.m_texCoordStart.y;
    end = buttonInfo.m_texCoordEnd.y;
}

void Logic::Button::updateOnPress(int posX, int posY)
{
	if (buttonInfo.m_rek.Contains(posX, posY))
	{
		m_CallBack();
	}
}

void Logic::Button::HoverOver(int posX, int posY)
{
    if (buttonInfo.m_rek.Contains(posX, posY))
    {
        if (!highlighted)
        {
            buttonInfo.m_texCoordStart.y -= buttonInfo.activeoffset;

            buttonInfo.m_texCoordEnd.y = buttonInfo.m_texCoordEnd.y - buttonInfo.activeoffset;
            highlighted = true;
        }

    }
    else
    {
        if (highlighted)
        {
            buttonInfo.m_texCoordStart.y = start;
            buttonInfo.m_texCoordEnd.y = end;
            highlighted = false;
        }
        
    }

}

bool Logic::Button::animationTransition(float dt, float maxAnimationTime, bool forward)
{
	m_animationTime += dt * (1 / maxAnimationTime);
	if (m_animationTime > 1)
	{
		m_animationTime = 1;
	}
	if (forward)
	{
		DirectX::SimpleMath::Vector2 temp = DirectX::SimpleMath::Vector2::Lerp(m_animationStart, m_animationEnd, m_animationTime);
		buttonInfo.m_rek = DirectX::SimpleMath::Rectangle(temp.x, temp.y, buttonInfo.m_rek.width, buttonInfo.m_rek.height);

		if (buttonInfo.m_rek.x == m_animationEnd.x && buttonInfo.m_rek.y == m_animationEnd.y)
		{
			m_animationTime = 0;
			return true;
		}
	}
	else
	{
		DirectX::SimpleMath::Vector2 temp = DirectX::SimpleMath::Vector2::Lerp(m_animationEnd, m_animationStart, m_animationTime);
		buttonInfo.m_rek = DirectX::SimpleMath::Rectangle(temp.x, temp.y, buttonInfo.m_rek.width, buttonInfo.m_rek.height);

		if (buttonInfo.m_rek.x == m_animationStart.x && buttonInfo.m_rek.y == m_animationStart.y)
		{
			m_animationTime = 0;
			return true;
		}
	}
	
	return false;
}

Graphics::ButtonInfo *Logic::Button::getButtonInfo()
{
	return &this->buttonInfo;
}
