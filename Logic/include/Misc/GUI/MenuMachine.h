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
#include <Graphics\include\Renderer.h>

namespace Logic
{
	class MenuMachine
	{
	private:

		std::map<GameState, MenuState*> m_menuStates;
		bool m_pressed;
		MenuState* m_currentActiveMenu;
		GameState m_currentActiveState;
		GameState m_stateToBe;
		bool m_forward;
		std::string* m_highScoreNamePTR;
		std::string m_highScoreName;
		bool m_typing;
        bool blinkMarker;
        float  blinkTimer;
        float deleteCharCD;
		int m_cardUpgrade;
	public:
		MenuMachine();
		MenuMachine(std::string* highScoreNamePTR);
		~MenuMachine();
		void initialize(GameState state);	//< Load menu layout from file
		void clear();						//< Clears current menu layout
		void update(float dt);
        void render(Graphics::Renderer& renderer);
		void showMenu(GameState state);		//< Creates a menu layout
		GameState currentState();
		void setStateToBe(GameState);
		GameState getStateToBe();
		int getChoiceUpgrade();

		void buttonClick0();
		void buttonClick1();
		void buttonClick2();
		void buttonClick3();
		void buttonClick4();

		void buttonClick5();

		void buttonClick6();

		void buttonClick7();

	};
}

#endif // !MENUMACHINE_H
