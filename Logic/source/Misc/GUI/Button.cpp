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