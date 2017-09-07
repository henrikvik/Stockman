#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Misc\Enums.h"

namespace Logic
{
	class StateMachine
	{
	public:
		StateMachine();
		~StateMachine();

		void setState(GameState state);
		GameState getState();

	private:
		GameState m_currentState;
	};
}

#endif // !STATEMACHINE_H