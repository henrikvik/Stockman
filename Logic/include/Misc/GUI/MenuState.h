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
		std::vector<Button*> m_buttons;		//< List of buttons on current menu
		std::string m_menuTexture;			//< file path for the menu background ska bytas till enum
	public:
		MenuState();
		~MenuState();
		void initialize();
	};
}

#endif