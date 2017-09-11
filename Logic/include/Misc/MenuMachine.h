#ifndef MENUMACHINE_H
#define MENUMACHINE_H

#pragma region Comment

/*
	Class: MenuMachine
	Author:

	Description:
	This class creates menus and handles switching between different menu layouts.

*/

#pragma endregion Description of class


#include <vector>
#include <map>
#include "Misc\Enums.h"
#include "Entity\Object.h"

namespace Logic
{
	class MenuMachine
	{
	private:
		struct MenuState
		{
			std::vector<Object> m_buttons;		//< List of buttons on current menu
			std::string m_menuTexture;			//< file path for the menu background
		};

		std::map<GameState, MenuState*> m_menuStates;

		MenuState* currentActive;
	public:
		MenuMachine();
		~MenuMachine();

		void showMenu(GameState state);		//< Creates a menu layout
		void removeMenu();					//< Removes current menu layout
		void readMenuLayoutFromFile();		//< Load menu layout from file
	

	};
}

#endif // !MENUMACHINE_H
