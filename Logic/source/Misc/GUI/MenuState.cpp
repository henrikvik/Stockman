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
	for (auto const& struc : buttonStruct)
	{
		DirectX::SimpleMath::Vector2 pos(struc.xPos, struc.yPos);
		DirectX::SimpleMath::Vector2 texCoordStart(struc.xTexStart, struc.yTexStart);
		DirectX::SimpleMath::Vector2 texCoordEnd(struc.xTexEnd, struc.yTexEnd);
		m_buttons.push_back(newd Button());
		m_buttons.at(m_buttons.size() - 1)->initialize(pos, texCoordStart, texCoordEnd, struc.height, struc.width, struc.texture, struc.m_CallBackFunction);
		m_menu.m_buttons.push_back(&m_buttons.at(m_buttons.size() - 1)->getButtonInfo());
	}
}

void Logic::MenuState::updateOnPress(int posX, int posY)
{
	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons.at(i)->updateOnPress(posX, posY);
	}
}
