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

void Logic::Button::animationTransition(float dt)
{
	
	DirectX::SimpleMath::Vector2 temp = DirectX::SimpleMath::Vector2::Lerp({ buttonInfo.m_rek.x, buttonInfo.m_rek.y }, m_animationEnd, dt);
	buttonInfo.m_rek = DirectX::SimpleMath::Rectangle(temp.x, temp.y, buttonInfo.m_rek.width, buttonInfo.m_rek.height);

}

Graphics::ButtonInfo *Logic::Button::getButtonInfo()
{
	return &this->buttonInfo;
}
