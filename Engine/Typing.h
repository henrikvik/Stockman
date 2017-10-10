#ifndef TYPING_H
#define TYPING_H

class Typing
{
private:
	static Typing* instance;
	char m_symbol;
	Typing();
public:
	static Typing* getInstance();
	char getSymbol();
	void setSymbol(char symbol);
};
#endif