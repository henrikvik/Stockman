#include "Typing.h"

Typing* Typing::instance = 0;

Typing::Typing()
{
	m_symbol = 0;
}

Typing::~Typing()
{
}

Typing* Typing::getInstance()
{
	if (instance == 0)
	{
		instance = new Typing();
	}
	return instance;
}

char Typing::getSymbol()
{
	char toSend = 0;
	if (m_symbol != 0)
	{
		toSend = m_symbol;
		m_symbol = 0;
	}
	return toSend;
}

void Typing::setSymbol(char symbol)
{
	m_symbol = symbol;
}

void Typing::releaseInstance()
{
	delete instance;
}
