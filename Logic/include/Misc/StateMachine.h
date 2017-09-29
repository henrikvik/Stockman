#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#pragma region Comment

/*
	Class: StateMachine
	Author:

	Description:
	Handles different game and menu states.

*/

#pragma endregion Description of class


#include "Misc\Enums.h"

namespace Logic
{
	class StateMachine
	{
	public:
		StateMachine();
		~StateMachine();

		void setState(GameState state);		//< Set current state
		GameState getState();				//< Returns current state

	private:
		GameState m_currentState;			//< Current state
	};
}

#endif // !STATEMACHINE_H