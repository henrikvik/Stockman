#ifndef MENUMACHINE_H
#define MENUMACHINE_H

#pragma region Comment

/*
Class: MenuMachine
Author: Emanuel Bjurman & Andreas Eriksson

Description:
This class creates menus and handles switching between different menu layouts.

*/

#pragma endregion Description of class


#include <map>
#include "Misc\Enums.h"
#include "MenuState.h"

namespace Logic
{
	class MenuMachine
	{
	private:
		std::map<GameState, MenuState*> m_menuStates;

		MenuState* currentActive;
	public:
		MenuMachine();
		~MenuMachine();
		void initialize();					//< Load menu layout from file
		void clear();						//< Clears current menu layout

		void showMenu(GameState state);		//< Creates a menu layout
		MenuState* currentMenu();


	};
}

#endif // !MENUMACHINE_H
