#include "Misc\GUI\MenuMachine.h"
#include <iostream>
using namespace Logic;

MenuMachine::MenuMachine()
{
	pressed = false;
	currentActiveMenu = nullptr;
	currentActiveState = gameStateMenuMain;

}


MenuMachine::~MenuMachine()
{
	for (const auto& a : m_menuStates)
	{
		delete a.second;
	}
}

void Logic::MenuMachine::initialize(GameState state)
{
	MenuMachine m;
	MenuState* test = new MenuState();
	test->initialize(std::bind(&MenuMachine::buttonClick0, this));
	m_menuStates[gameStateMenuMain] = test;

	MenuState* test1 = new MenuState();
	test1->initialize(std::bind(&MenuMachine::buttonClick1, this));
	m_menuStates[gameStateMenuSettings] = test1;

	MenuState* test2 = new MenuState();
	test2->initialize(std::bind(&MenuMachine::buttonClick2, this));
	m_menuStates[gameStateGame] = test2;

	showMenu(gameStateMenuMain);

	//accquire menu state
	//accquires position
	//accquires its width and height
	//accquires texture
}

void Logic::MenuMachine::clear() 
{
	currentActiveMenu = nullptr;
}

void Logic::MenuMachine::update()
{
	auto Mouse = DirectX::Mouse::Get().GetState();

	if (Mouse.leftButton && !pressed)
	{
		pressed = true;
		currentActiveMenu->updateOnPress(Mouse.x, Mouse.y);
	}
	else if (!Mouse.leftButton && pressed)
	{
		pressed = false;

	}
}

void Logic::MenuMachine::showMenu(GameState state)
{
	if (m_menuStates.find(state) != m_menuStates.end())
	{
		currentActiveMenu = m_menuStates.at(state);
		currentActiveState = state;
	}
	else
	{
		currentActiveMenu = m_menuStates.at(gameStateMenuMain); //change to error state
		currentActiveState = gameStateMenuMain;
	}
}

GameState Logic::MenuMachine::currentState()
{
	return currentActiveState;
}

void Logic::MenuMachine::buttonClick0()
{
	showMenu(gameStateMenuSettings);
	std::cout << "Left Trigger: Switched To Menu State 1";
}

void Logic::MenuMachine::buttonClick1()
{
	showMenu(gameStateGame);
	std::cout << "Left Trigger: Switched To Menu State 2";
}

void Logic::MenuMachine::buttonClick2()
{
	showMenu(gameStateMenuMain);
	std::cout << "Left Trigger: Switched To Menu State 0";
}
