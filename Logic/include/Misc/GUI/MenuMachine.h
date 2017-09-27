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
#include <Misc\Enums.h>
#include <Misc\GUI\MenuState.h>

namespace Logic
{
	class MenuMachine
	{
	private:

		std::map<GameState, MenuState*> m_menuStates;
		bool pressed;
		MenuState* currentActiveMenu;
		GameState currentActiveState;
	public:

		MenuMachine();
		~MenuMachine();
		void initialize(GameState state);	//< Load menu layout from file
		void clear();						//< Clears current menu layout
		void update();

		void showMenu(GameState state);		//< Creates a menu layout
		GameState currentState();


		void buttonClick0();
		void buttonClick1();

		void buttonClick2();


		


	};
}

#endif // !MENUMACHINE_H
