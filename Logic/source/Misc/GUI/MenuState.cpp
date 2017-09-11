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

void Logic::MenuState::initialize()
{
	m_buttons.push_back(new Button());
}

