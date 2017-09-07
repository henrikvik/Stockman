#include "../Misc/Card.h"

using namespace Logic;

Card::Card()
{
	m_name = "";
	m_description = "";
	m_statusID = -1;
}

Card::Card(std::string name, std::string description, int statusID)
{
	m_name = name;
	m_description = description;
	m_statusID = statusID;
}

Card::~Card() { }

std::string Card::getName() const { return m_name; }
std::string Card::getDescription() const { return m_description; }
int Card::getStatusID() const { return m_statusID; }
