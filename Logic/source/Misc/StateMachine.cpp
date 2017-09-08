#include "Misc\StateMachine.h"

using namespace Logic;

StateMachine::StateMachine()
{
}


StateMachine::~StateMachine()
{
}

void StateMachine::setState(GameState state)
{
	m_currentState = state;
}

GameState StateMachine::getState()
{
	return m_currentState;
}