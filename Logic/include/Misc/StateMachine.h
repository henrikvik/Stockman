#ifndef STATEMACHINE_H
#define STATEMACHINE_H

namespace Logic
{
	class StateMachine
	{
	public:
		StateMachine();
		~StateMachine();

		enum State
		{
			stateGame,
			stateLoading,
			stateMenuMain,
			stateMenuSettings
		};

		void setState(State state);
		State getState();

	private:
		State m_currentState;
	};
}

#endif // !STATEMACHINE_H