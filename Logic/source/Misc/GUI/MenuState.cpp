#include "..\..\..\include\Misc\GUI\MenuState.h"


Logic::MenuState::MenuState()
{
}

Logic::MenuState::~MenuState()
{
	for (int i = 0; i < m_buttons.size(); i++)
	{
		delete m_buttons[i];
	}
}

void Logic::MenuState::initialize(std::vector<ButtonStruct> buttonStruct, std::string background)
{
	/*DirectX::SimpleMath::Vector2 pos(xPos, yPos);
	DirectX::SimpleMath::Vector2 texCoordStart(xTexStart, yTexStart);
	DirectX::SimpleMath::Vector2 texCoordEnd(xTexEnd, yTexEnd);
	m_buttons.push_back(newd Button());
	m_buttons.at(m_buttons.size() - 1)->initialize(pos, texCoordStart, texCoordEnd, height, width, texture, callBack);

	m_menu.m_buttons.push_back(&m_buttons.at(0)->getButtonInfo());*/
}

void Logic::MenuState::updateOnPress(int posX, int posY)
{
	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons.at(i)->updateOnPress(posX, posY);
	}
}
