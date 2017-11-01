#include <Misc\GUI\Button.h>
using namespace Logic;

Button::Button()
{
}

Button::~Button()
{
}

void Button::initialize(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 texCoordStart, DirectX::SimpleMath::Vector2 texCoordEnd, float offset, float height, float width, int textureIndex, std::function<void(void)> callback)
{
	m_buttonInfo.m_rek = DirectX::SimpleMath::Rectangle((long)pos.x, (long)pos.y, (long)width, (long)height);
	m_buttonInfo.m_texCoordStart = texCoordStart;
	m_buttonInfo.m_texCoordEnd = texCoordEnd;
	m_buttonInfo.activeoffset = offset;
	m_buttonInfo.textureIndex = textureIndex;

	m_animationStart = DirectX::SimpleMath::Vector2(pos.x, pos.y);
	m_animationEnd = DirectX::SimpleMath::Vector2(0 - width, pos.y);
	m_animationTime = 0;
	m_start = m_buttonInfo.m_texCoordStart.y;
	m_end = m_buttonInfo.m_texCoordEnd.y;

	m_callback = callback;
	m_highlighted = false;
}

void Button::updateOnPress(int posX, int posY)
{
	if (m_buttonInfo.m_rek.Contains(posX, posY))
	{
		m_callback();
	}
}

void Button::hoverOver(int posX, int posY)
{
    if (m_buttonInfo.m_rek.Contains(posX, posY))
    {
        if (!m_highlighted)
        {
            m_buttonInfo.m_texCoordStart.y -= m_buttonInfo.activeoffset;

			m_buttonInfo.m_texCoordEnd.y = m_buttonInfo.m_texCoordEnd.y - m_buttonInfo.activeoffset;
            m_highlighted = true;
        }

    }
    else
    {
        if (m_highlighted)
        {
            m_buttonInfo.m_texCoordStart.y = m_start;
            m_buttonInfo.m_texCoordEnd.y = m_end;
			m_highlighted = false;
        }
    }
}

bool Button::animationTransition(float dt, float maxAnimationTime, bool forward)
{
	m_animationTime += dt * (1 / maxAnimationTime);
	bool done = false;

	if (m_animationTime >= 1)
	{
		m_animationTime = 0;
		done = true;
	}
 
	DirectX::SimpleMath::Vector2 lerpResult;
	if (forward)
		lerpResult = DirectX::SimpleMath::Vector2::Lerp(m_animationStart, m_animationEnd, done ? 1 : m_animationTime);
	else
		lerpResult = DirectX::SimpleMath::Vector2::Lerp(m_animationEnd, m_animationStart, done ? 1 : m_animationTime);
	
	m_buttonInfo.m_rek = DirectX::SimpleMath::Rectangle((long)lerpResult.x, (long)lerpResult.y,
		m_buttonInfo.m_rek.width, m_buttonInfo.m_rek.height);

	return done;
}

Graphics::ButtonInfo* Button::getButtonInfo()
{
	return &m_buttonInfo;
}

void Button::setStartAndEnd(float start, float end)
{
    m_start = start;
    m_end = end;
}
