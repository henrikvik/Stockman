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
#include <Misc\GUI\Button.h>


#pragma endregion Description of class

namespace Logic
{
	class MenuState
	{
	private:
		Graphics::MenuInfo m_menu;
		std::vector<Button*> m_buttons;		//< List of buttons on current menu
	public:

		struct ButtonStruct
		{
			float xPos;
			float yPos;
			float xTexStart;
			float yTexStart;
			float xTexEnd;
			float yTexEnd;
			float height;
			float width;
			std::string texture;
			std::function<void(void)> m_CallBackFunction; //must change to a function aqquiering
		};

		MenuState();
		~MenuState();
		void initialize(std::vector<ButtonStruct> buttonStruct, std::string background);
		void updateOnPress(int posX, int posY);
	};
}

#endif