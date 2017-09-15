#ifndef MENUSTATE_H
#define MENUSTATE_H

#pragma region Comment

/*
Class: MenuState
Author: Andreas Eriksson

Description:
This class creates menu states.

*/
#pragma endregion Description of class

#include <vector>
#include "Button.h"


#pragma endregion Description of class

namespace Logic
{
	class MenuState
	{
	private:
		Graphics::MenuInfo m_menu;
		std::vector<Button*> m_buttons;		//< List of buttons on current menu
	public:
		MenuState();
		~MenuState();
		void initialize(std::function<void(void)> callBack);
		void updateOnPress(int posX, int posY);
	};
}

#endif