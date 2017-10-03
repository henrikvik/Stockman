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
    }
}

void Logic::MenuState::updateOnPress(int posX, int posY)
{
	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons.at(i)->updateOnPress(posX, posY);
	}
}

bool Logic::MenuState::animationTransition(float dt, float maxAnimationTime, bool forward)
{
	bool done;
	for (int i = 0; i < m_buttons.size(); i++)
	{
		done = m_buttons.at(i)->animationTransition(dt, maxAnimationTime, forward);
	}

	if (m_buttons.size() == 0)
	{
		done = true;
	}

	return done;
}

Graphics::MenuInfo Logic::MenuState::getMenuInfo()
{
    m_menu.m_buttons.clear();
    for (size_t i = 0; i < this->m_buttons.size(); i++)
    {
        auto temp = m_buttons.at(i)->getButtonInfo();
        m_menu.m_buttons.push_back(*temp);
    }
    return m_menu;
}

