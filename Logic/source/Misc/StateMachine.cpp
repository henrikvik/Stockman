#include "Misc\StateMachine.h"

using namespace Logic;

StateMachine::StateMachine()
{
}


StateMachine::~StateMachine()
{
}

void StateMachine::setState(State state)
{
	m_currentState = state;
}

StateMachine::State StateMachine::getState()
{
	return m_currentState;
}