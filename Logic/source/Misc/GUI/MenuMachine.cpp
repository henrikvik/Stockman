#include "Misc\GUI\MenuMachine.h"

using namespace Logic;

MenuMachine::MenuMachine()
{
}


MenuMachine::~MenuMachine()
{
	for (const auto& a : m_menuStates)
	{
		delete a.second;
	}
}

void Logic::MenuMachine::initialize()
{
	MenuState* test = new MenuState();
	test->initialize();
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

void Logic::MenuMachine::update()
{
	currentActive->update();
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

MenuState* Logic::MenuMachine::currentMenu()
{
	return currentActive;
}


