#include "Misc\GUI\MenuMachine.h"

using namespace Logic;

MenuMachine::MenuMachine()
{
}


MenuMachine::~MenuMachine()
{
	for (const auto& a : m_menuStates)
	{
		for (int i = 0; i < a.second->m_buttons.size(); i++)
		{
			delete a.second->m_buttons[i];
		}
		delete a.second;
	}
}

void Logic::MenuMachine::initialize()
{
	MenuState* test = new MenuState();
	test->m_buttons.push_back(new Object());
	m_menuStates[gameStateMenuMain] = test;
	//accquire menue state
	//accquires position
	//accquires its width and height
	//accquires texture
}

void Logic::MenuMachine::clear() 
{
	currentActive = nullptr;
}

void Logic::MenuMachine::showMenu(GameState state)
{
	if (m_menuStates.find(state) != m_menuStates.end())
	{
		currentActive = m_menuStates.at(state);
	}
	else
	{
		currentActive = nullptr;
	}
}

MenuMachine::MenuState* Logic::MenuMachine::getMenuState()
{
	return currentActive;
}


