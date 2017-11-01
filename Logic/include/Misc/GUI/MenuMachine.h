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
        std::pair<int, int> m_selectedSkills;
        void selectSkillButton(int id);
        bool replaceSkill(int id);

		float m_mouseSense = 0.0f;
		float m_FOV = 0.0f;
		float m_masterSound = 0.0f;
		float m_SFX = 0.0f;
		float m_music = 0.0f;
		bool m_windowed = false;
		bool m_mute = false;
	public:
		MenuMachine(std::string* highScoreNamePTR);
		~MenuMachine();
		void initialize(GameState state);	//< Load menu layout from file
		void clear();						//< Clears current menu layout
		void update(float dt);
		void render(Graphics::Renderer & renderer, std::string highScore[10]);
		void showMenu(GameState state);		//< Creates a menu layout
		GameState currentState();
		void setStateToBe(GameState);
		GameState getStateToBe();
		void startGame();
		void startSettings();
		void startMainMenu();
		void quitGame();
		void writing();
		void chooseUpgrade1();
		void chooseUpgrade2();
		void chooseUpgrade3();
		void plusSense();
		void minusSense();
		void plusMaster();
		void minusMaster();
		void plusSFX();
		void minusSFX();
		void muteUnmute();
		void plusFOV();
		void minusFOV();
		void windowed();
		void showHighscore();
		int getChoiceUpgrade();
        MenuState* getActiveMenu();
        std::pair<int, int>* getSkillPick();



        void buttonSkillPick1();
        void buttonSkillPick2();
        void buttonSkillPick3();

	};
}

#endif // !MENUMACHINE_H
