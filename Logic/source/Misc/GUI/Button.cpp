#include "..\..\..\include\Misc\GUI\Button.h"

Logic::Button::Button()
{
}

Logic::Button::~Button()
{
}

void Logic::Button::initialize(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 texCoordStart, DirectX::SimpleMath::Vector2 texCoordEnd, float height, float width, std::string texture, std::function<void(void)> callBack)
{
	buttonInfo.m_rek = DirectX::SimpleMath::Rectangle(pos.x, pos.y, width, height);
	m_animationStart = DirectX::SimpleMath::Vector2(pos.x, pos.y);
	m_animationEnd = DirectX::SimpleMath::Vector2(800, 800);
	buttonInfo.m_texCoordStart = texCoordStart;
	buttonInfo.m_texCoordEnd = texCoordEnd;
	buttonInfo.m_texture = texture;
	m_CallBack = callBack;
}

void Logic::Button::updateOnPress(int posX, int posY)
{
	if (buttonInfo.m_rek.Contains(posX, posY))
	{
		m_CallBack();
	}
}

bool Logic::Button::animationTransition(float dt)
{
	m_animationTime += dt / 10;
	DirectX::SimpleMath::Vector2 temp = DirectX::SimpleMath::Vector2::Lerp(m_animationStart, m_animationEnd, m_animationTime);
	buttonInfo.m_rek = DirectX::SimpleMath::Rectangle(temp.x, temp.y, buttonInfo.m_rek.width, buttonInfo.m_rek.height);

	if (buttonInfo.m_rek.x == m_animationEnd.x && buttonInfo.m_rek.y == m_animationEnd.y)
	{
		buttonInfo.m_rek = DirectX::SimpleMath::Rectangle(temp.x, temp.y, buttonInfo.m_rek.width, buttonInfo.m_rek.height);
		return true;
	}
	return false;
}

Graphics::ButtonInfo *Logic::Button::getButtonInfo()
{
	return &this->buttonInfo;
}
