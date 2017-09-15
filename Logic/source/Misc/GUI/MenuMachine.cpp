#include "Misc\GUI\MenuMachine.h"
#include <iostream>
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
	MenuMachine m;
	MenuState* test = new MenuState();
	test->initialize(std::bind(&MenuMachine::buttonClick0, this));
	m_menuStates[gameStateMenuMain] = test;

	MenuState* test1 = new MenuState();
	test1->initialize(std::bind(&MenuMachine::buttonClick1, this));
	m_menuStates[gameStateMenuSettings] = test1;
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

void Logic::MenuMachine::buttonClick0()
{
	showMenu(gameStateMenuSettings);
	std::cout << "Left Trigger: Switched To Menu State 1";
}

void Logic::MenuMachine::buttonClick1()
{
	showMenu(gameStateMenuMain);
	std::cout << "Left Trigger: Switched To Menu State 0";
}

