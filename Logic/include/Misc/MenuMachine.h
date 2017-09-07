#ifndef MENUMACHINE_H
#define MENUMACHINE_H

#pragma region Comment

/*
	Class: MenuMachine
	Author: Emanuel Bjurman

	Description:
	This class creates menus and handles switching between different menu layouts.

*/

#pragma endregion Description of class


#include <vector>
#include "Misc\Enums.h"
#include "Entity\Object.h"

namespace Logic
{
	class MenuMachine
	{
	public:
		MenuMachine();
		~MenuMachine();

		void showMenu(GameState state);		//< Creates a menu layout
		void removeMenu();					//< Removes current menu layout
	private:
		std::vector<Object> m_buttons;		//< List of buttons on current menu

		void readMenuLayoutFromFile();		//< Load menu layout from file
	};
}

#endif // !MENUMACHINE_H
